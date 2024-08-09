#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "token.h"
#include "common.h"
#include "scanner.h"
#include "String.h"

#define EOF (-1)
#define ERROR (-2)
typedef int Code_Point;

static int File_Descriptor = STDIN_FILENO;
static struct Position File_Position;
static struct String Extent_String;
static struct String Error_Message;

static Code_Point Character_Buffer[3] = { -1 };
static bool End_Of_File(void);
static Code_Point Next_Character(void);
static Code_Point Peek_Character(void);
static bool Match_Character(Code_Point character);
static bool Match_Character_Range(Code_Point from, Code_Point to, Code_Point *character);

int Open (int fd) {
    File_Descriptor = fd;
    
    // reset position/buffers
    File_Position.Line = 1;
    File_Position.Column = 1;
    foreach(i, 0, Character_Buffer){
        Character_Buffer[i] = -1;
    }
    
    // prime the character buffer
    char buffer[arr_sizeof(Character_Buffer) - 1];
    let result = read(fd, &buffer, sizeof(buffer));
    if (result < 0){
        close(fd);
        return -1;
    }

    for(let i = 0; i < result; i += 1){
        Character_Buffer[i + 1] = buffer[i];
    }
    
    return File_Descriptor;
}

int Open_File (const char* path){
    return Open(open(path, O_RDONLY));
}

int Close (void) {
    return close(File_Descriptor);
}

static bool End_Of_File(void){
    return Character_Buffer[1] == EOF;
}

static Code_Point Next_Character(void){
    if(End_Of_File())
        return EOF;

    let current = Character_Buffer[1];
    foreach(i, 1, Character_Buffer){
        Character_Buffer[i - 1] = Character_Buffer[i];
    }

    char c;
    let result = read(File_Descriptor, &c, sizeof(c));
    if(result <= 0)
        ridx(Character_Buffer, -1) = EOF;
    else
        ridx(Character_Buffer, -1) = c;
    
    if (Character_Buffer[0] == '\n'){
        File_Position.Line += 1;
        File_Position.Column = 1;
    } else if(Character_Buffer[0] >= 0) {
        File_Position.Column += 1;
    }

    return current;
}

static Code_Point Peek_Character(void){
    return Character_Buffer[1];
}

static bool Match_Character(Code_Point character){
    if(End_Of_File()){
        return character == EOF;
    }
    if(Peek_Character() != character){
        return false;
    }
    
    Next_Character();
    return true;
}

static bool Match_Character_Range(Code_Point from, Code_Point to, Code_Point *character){
    if(End_Of_File() || from == EOF || to == EOF || from > to)
        return false;
    let peek = Peek_Character();
    if(peek < from || peek > to)
        return false;
    
    if(NULL != character)
        *character = Next_Character();
    else
        Next_Character();
    return true;
}

static bool Is_Numeric(Code_Point character){
    return character >= '0' && character <= '9';
}

static bool Is_Alphabetic(Code_Point character){
    return (character >= 'A' && character <= 'Z')
        || (character >= 'a' && character <= 'z');
}

static bool Error_Token(const Token_Ptr token, struct Position position, const String_Ptr message){
    token->Token.Kind = TOKEN_ERROR;
    token->Token.Position = position;
    token->Error_Token.Error_Position = File_Position;
    Move_String(&token->Error_Token.Message, message);
    return false;
}

static bool Format_Error(const Token_Ptr token, struct Position position, const char *fmt, ...){
    char buffer[1024];
    va_list arg;
    va_start(arg, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, arg);
    va_end(arg);

    return Error_Token(token, position, New_String_Text_Lit(&Error_Message, buffer));
}

static bool Simple_Token(const Token_Ptr token, struct Position position, enum Token_Kind kind){
    token->Token.Kind = kind;
    token->Token.Position = position;
    return true;
}

static void Line_Comment(){
    New_String(&Extent_String);
    Append_Text(&Extent_String, 2, "//");
    while(!Match_Character('\n') && !Match_Character(EOF)){
        let current = Next_Character();
        Append_Char(&Extent_String, current);
    }
    printf("Line Comment: \"%s\"\n", Extent_String.Content);
    Free_String(&Extent_String);
}

static bool String_Token(const Token_Ptr token, struct Position position){
    New_String(&Extent_String);

    loop {
        let current = Next_Character();
        switch (current){
            case EOF: goto found_eof;
            case '"': goto done;
            case '\\':
                current = Next_Character();
                switch (current){
                    case EOF: goto found_eof;
                    default:
                        return Format_Error(token, position, "Found illegal character in escape sequqnce: %c (%d)", current, current);
                    case '0': Append_Char(&Extent_String, '\0'); continue;
                    case 'a': Append_Char(&Extent_String, '\a'); continue;
                    case 'b': Append_Char(&Extent_String, '\b'); continue;
                    case 'e': Append_Char(&Extent_String, 0x1b); continue;
                    case 'f': Append_Char(&Extent_String, '\f'); continue;
                    case 'n': Append_Char(&Extent_String, '\n'); continue;
                    case 'r': Append_Char(&Extent_String, '\r'); continue;
                    case 't': Append_Char(&Extent_String, '\t'); continue;
                    case 'v': Append_Char(&Extent_String, '\v'); continue;
                    case 'u': {
                        Code_Point result = 0;
                        Code_Point buffer;
                        let i = 0;
                        for(; i < 4; i += 1){
                            if(Match_Character_Range('0', '9', &buffer))
                                result = (result << 4) | (buffer - '0');
                            else if(Match_Character_Range('A', 'F', &buffer))
                                result = (result << 4) | (buffer - 'A' + 10);
                            else if(Match_Character_Range('a', 'f', &buffer))
                                result = (result << 4) | (buffer - 'a' + 10);
                            else if(Match_Character(EOF))
                                goto found_eof;
                            else
                                return Format_Error(token, position, "Found illegal character in Unicode 2-byte escape sequence: %c (%d)", Peek_Character(), Peek_Character());
                        }

                        if(result > UINT8_MAX)
                            return Format_Error(token, position, "Escape sequence is too large: %d", result);
                        Append_Char(&Extent_String, (char) result);
                    } continue;
                    case 'U': {
                        Code_Point result = 0;
                        Code_Point buffer;
                        let i = 0;
                        for(; i < 8; i += 1){
                            if(Match_Character_Range('0', '9', &buffer))
                                result = (result << 4) | (buffer - '0');
                            else if(Match_Character_Range('A', 'F', &buffer))
                                result = (result << 4) | (buffer - 'A' + 10);
                            else if(Match_Character_Range('a', 'f', &buffer))
                                result = (result << 4) | (buffer - 'a' + 10);
                            else if(Match_Character(EOF))
                                goto found_eof;
                            else
                                return Format_Error(token, position, "Found illegal character in Unicode 4-byte escape sequence: %c (%d)", Peek_Character(), Peek_Character());
                        }

                        if(result > UINT8_MAX)
                            return Format_Error(token, position, "Escape sequence is too large: %d", result);
                        Append_Char(&Extent_String, (char) result);
                    } continue;
                    case 'x': {
                        Code_Point result = 0;
                        Code_Point buffer;
                        for(let i = 0; i < 4; i += 1){
                            if(Match_Character_Range('0', '9', &buffer))
                                result = (result << 4) | (buffer - '0');
                            else if(Match_Character_Range('A', 'F', &buffer))
                                result = (result << 4) | (buffer - 'A' + 10);
                            else if(Match_Character_Range('a', 'f', &buffer))
                                result = (result << 4) | (buffer - 'a' + 10);
                            else if(i == 0 && Match_Character(EOF))
                                goto found_eof;
                            else if(i == 0)
                                return Format_Error(token, position, "Invalid hex character in variable-width escape sequence: %c (%d)", Peek_Character(), Peek_Character());
                            else if(Match_Character(EOF) || Match_Character('"'))
                                break;
                        }

                        if(result > UINT8_MAX)
                            return Format_Error(token, position, "Escape sequence is too large: %d", result);
                        Append_Char(&Extent_String, (char) result);
                    } continue;
                    case '\'':
                    case '"':
                    case '\\':
                        break;
                }
                // fall through
            default:
                Append_Char(&Extent_String, current);
                continue;
        }

        found_eof:
        return Format_Error(token, position, "Found EOF while lexing string.");
    }

    done:;
    let _token = (Text_Token_Ptr) token;
    _token->Base.Kind = TOKEN_STRING;
    _token->Base.Position = position;
    Move_String(&_token->Text, &Extent_String);
    return true;
}

static bool Number_Token(const Token_Ptr token, struct Position position, Code_Point current){
    New_String(&Extent_String);
    Append_Char(&Extent_String, current);
    
    while(Match_Character_Range('0', '9', &current)){
        Append_Char(&Extent_String, current);
    }

    if(Match_Character('.')){
        Append_Char(&Extent_String, '.');
        while(Match_Character_Range('0', '9', &current)){
            Append_Char(&Extent_String, current);
        }
    }

    token->Token.Kind = TOKEN_NUMBER;
    token->Token.Position = position;
    token->Number_Token.Value = atof(Extent_String.Content);
    Free_String(&Extent_String);
    return true;
}

static bool Text_Token(const Token_Ptr token, struct Position position, Code_Point current){
    New_String(&Extent_String);
    Append_Char(&Extent_String, current);
}

bool Next_Token (const Token_Ptr token) {
    if(End_Of_File()){
        Simple_Token(token, File_Position, TOKEN_EOF);
        return false;
    }

    loop {
        let position = File_Position;
        let current = Next_Character();
        switch (current){
            case EOF: Simple_Token(token, position, TOKEN_EOF); return false;
            case '(': return Simple_Token(token, position, TOKEN_LPAREN);
            case ')': return Simple_Token(token, position, TOKEN_RPAREN);
            case '{': return Simple_Token(token, position, TOKEN_LBRACE);
            case '}': return Simple_Token(token, position, TOKEN_RBRACE);
            case ',': return Simple_Token(token, position, TOKEN_COMMA);
            case '.': return Simple_Token(token, position, TOKEN_DOT);
            case '-': return Simple_Token(token, position, TOKEN_MINUS);
            case '+': return Simple_Token(token, position, TOKEN_PLUS);
            case ';': return Simple_Token(token, position, TOKEN_SEMICOLON);
            case '*': return Simple_Token(token, position, TOKEN_STAR);
            case '"': return String_Token(token, position);
            case '!':
                if(Match_Character('='))
                    return Simple_Token(token, position, TOKEN_BANG_EQUAL);
                return Simple_Token(token, position, TOKEN_BANG);
            case '=':
                if(Match_Character('='))
                    return Simple_Token(token, position, TOKEN_EQUAL_EQUAL);
                return Simple_Token(token, position, TOKEN_EQUAL);
            case '<':
                if(Match_Character('='))
                    return Simple_Token(token, position, TOKEN_LESS_EEQUAL);
                return Simple_Token(token, position, TOKEN_LESS);
            case '>':
                if(Match_Character('='))
                    return Simple_Token(token, position, TOKEN_GREATER_EQUAL);
                return Simple_Token(token, position, TOKEN_GREATER);
            case '/':
                if(!Match_Character('/'))
                    return Simple_Token(token, position, TOKEN_SLASH);
                Line_Comment();
                continue;
            case '\n': case ' ': case '\r': case '\t':
                continue;
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                return Number_Token(token, position, current);
            default:
                if(Is_Alphabetic(current))
                    return true; // TODO
                return Format_Error(token, position, "Unrecognized character: %c (%d)", current, current);
        }
    }
}
