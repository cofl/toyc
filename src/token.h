#ifndef __TOKEN_H__
#define __TOKEN_H__
#include "String.h"
#include "common.h"

enum Token_Kind {
    TOKEN_ERROR = 0,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_COMMA,
    TOKEN_DOT,
    TOKEN_MINUS,
    TOKEN_PLUS,
    TOKEN_SEMICOLON,
    TOKEN_SLASH,
    TOKEN_STAR,
    TOKEN_BANG,
    TOKEN_BANG_EQUAL,
    TOKEN_EQUAL,
    TOKEN_EQUAL_EQUAL,
    TOKEN_GREATER,
    TOKEN_GREATER_EQUAL,
    TOKEN_LESS,
    TOKEN_LESS_EEQUAL,
    TOKEN_IDENTIFIER,
    TOKEN_STRING,
    TOKEN_NUMBER,
    TOKEN_AND,
    TOKEN_CLASS,
    TOKEN_ELSE,
    TOKEN_FALSE,
    TOKEN_FUN,
    TOKEN_FOR,
    TOKEN_IF,
    TOKEN_NIL,
    TOKEN_OR,
    TOKEN_PRINT,
    TOKEN_RETURN,
    TOKEN_SUPER,
    TOKEN_THIS,
    TOKEN_TRUE,
    TOKEN_VAR,
    TOKEN_EOF = -1,
};

struct Any_Token {
    enum Token_Kind Kind;
    struct Position Position;
};
typedef struct Any_Token *Any_Token_Ptr;

struct Text_Token {
    struct Any_Token Base;
    struct String Text;
};
typedef struct Text_Token *Text_Token_Ptr;

struct Number_Token {
    struct Any_Token Base;
    double Value;
};
typedef struct Number_Token *Number_Token_Ptr;

struct Error_Token {
    struct Any_Token Base;
    struct Position Error_Position;
    struct String Message;
};
typedef struct Error_Token *Error_Token_Ptr;

union Token {
    struct Any_Token Token;
    struct Text_Token Text_Token;
    struct Error_Token Error_Token;
    struct Number_Token Number_Token;
};

typedef union Token Token, *Token_Ptr;

#endif
