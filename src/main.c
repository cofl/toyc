#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include "common.h"
#include "scanner.h"

void Run_File(const char *path){

    let f = fopen(path, "r");
    fseek(f, 0L, SEEK_END);
    
    let size = ftell(f);
    fseek(f, 0L, SEEK_SET);
    
    let buffer = talloc(char, size + 1);
    buffer[size] = '\0';
    fread(buffer, sizeof(char), size, f);
    
    //Run(buffer);
    free(buffer);
}

void Run_Prompt(){
    Token token;
    Open(STDIN_FILENO);
    bool result;
    do {
        result = Next_Token(&token);
        #define PrintToken(kind) case kind: printf("%s (%d, %d)\n", #kind, token.Token.Position.Line, token.Token.Position.Column); break;
        switch (token.Token.Kind){
            PrintToken(TOKEN_LPAREN);
            PrintToken(TOKEN_RPAREN);
            PrintToken(TOKEN_LBRACE);
            PrintToken(TOKEN_RBRACE);
            PrintToken(TOKEN_COMMA);
            PrintToken(TOKEN_DOT);
            PrintToken(TOKEN_MINUS);
            PrintToken(TOKEN_PLUS);
            PrintToken(TOKEN_SEMICOLON);
            PrintToken(TOKEN_SLASH);
            PrintToken(TOKEN_STAR);
            PrintToken(TOKEN_BANG);
            PrintToken(TOKEN_BANG_EQUAL);
            PrintToken(TOKEN_EQUAL);
            PrintToken(TOKEN_EQUAL_EQUAL);
            PrintToken(TOKEN_GREATER);
            PrintToken(TOKEN_GREATER_EQUAL);
            PrintToken(TOKEN_LESS);
            PrintToken(TOKEN_LESS_EEQUAL);
            PrintToken(TOKEN_IDENTIFIER);
            PrintToken(TOKEN_AND);
            PrintToken(TOKEN_CLASS);
            PrintToken(TOKEN_ELSE);
            PrintToken(TOKEN_FALSE);
            PrintToken(TOKEN_FUN);
            PrintToken(TOKEN_FOR);
            PrintToken(TOKEN_IF);
            PrintToken(TOKEN_NIL);
            PrintToken(TOKEN_OR);
            PrintToken(TOKEN_PRINT);
            PrintToken(TOKEN_RETURN);
            PrintToken(TOKEN_SUPER);
            PrintToken(TOKEN_THIS);
            PrintToken(TOKEN_TRUE);
            PrintToken(TOKEN_VAR);
            PrintToken(TOKEN_EOF);
            case TOKEN_STRING:
                printf("TOKEN_STRING (%d, %d) \"%s\"\n",
                    token.Token.Position.Line, token.Token.Position.Column,
                    token.Text_Token.Text.Content);
                break;
            case TOKEN_NUMBER:
                printf("TOKEN_NUMBER (%d, %d): %lf\n",
                    token.Token.Position.Line, token.Token.Position.Column,
                    token.Number_Token.Value);
                break;
            case TOKEN_ERROR:
                printf("TOKEN_ERROR (%d, %d)-(%d, %d): %s\n",
                    token.Token.Position.Line, token.Token.Position.Column,
                    token.Error_Token.Error_Position.Line, token.Error_Token.Error_Position.Column,
                    token.Error_Token.Message.Content);
                break;
        }
        #undef PrintToken
    } while(result);
}

int main(int argc, char **argv){
    if(argc > 2){
        fprintf(stderr, "Usage: toyc [script]\n");
        return 64;
    }

    if(argc == 2){
        Run_File (argv[1]);
    } else {
        Run_Prompt ();
    }

    return 0;
}
