#ifndef __STRING_H__
#define __STRING_H__
#include<sys/types.h>
#include<stdbool.h>
#include "common.h"

typedef struct String {
    char *Content;
    size_t Size;
    size_t Buffer_Size;
    bool Managed_Buffer;
} *String_Ptr;

void New_String(const String_Ptr string);

void New_Fixed_String(const String_Ptr string, size_t size, char buffer[size]);
#define New_Fixed_String_Lit(string, buffer) New_Fixed_String((string), sizeof(buffer), (buffer));

String_Ptr New_String_Text(const String_Ptr string, size_t max_length, const char *text);
#define New_String_Text_Lit(string, literal) New_String_Text((string), sizeof(literal), (literal))

void Append_Char(const String_Ptr string, char c);
void Append_Text(const String_Ptr string, size_t max_length, const char *text);
#define Append_Text_Lit(string, literal) Append_Text((string), sizeof(literal), (literal))

void Free_String(const String_Ptr string);
void Move_String(const String_Ptr dest, const String_Ptr src);

#endif
