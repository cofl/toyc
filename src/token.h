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
    TOKEN_LESS_EQUAL,
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

typedef struct Token {
    enum Token_Kind Kind;
    struct Position Position;
    union {
        struct String Text_Value;
        double Double_Value;
        struct {
            struct String Message;
            struct Position End_Position;
        } Error_Value;
    };
} Token, *Token_Ptr;

#endif
