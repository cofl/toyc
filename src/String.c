#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include "String.h"

void New_String(const String_Ptr string){
    string->Size = 0;
    string->Buffer_Size = 16;
    string->Content = talloc(char, 16);
    string->Content[0] = '\0';
    string->Managed_Buffer = true;
}

void New_Fixed_String(const String_Ptr string, size_t size, const char buffer[size]){
    string->Size = 0;
    string->Buffer_Size = size;
    string->Content = buffer;
    string->Content[0] = '\0';
    string->Managed_Buffer = false;
}

String_Ptr New_String_Text(const String_Ptr string, size_t max_length, const char *text){
    New_String(string);
    Append_Text(string, max_length, text);
    return string;
}

static void Extend_Buffer(const String_Ptr string, size_t new_size){
    if(string->Managed_Buffer){
        string->Content = trealloc(string->Content, new_size);
        string->Buffer_Size = new_size;
    } else {
        string->Managed_Buffer = true;
        
        let temp = talloc(char, string->Buffer_Size);
        strncpy(temp, string->Size, string->Content);
        string->Content = temp;
    }
}

void Append_Char(const String_Ptr string, char c){
    string->Content[string->Size] = c;
    string->Size += 1;
    if(string->Size == string->Buffer_Size)
        Extend_Buffer(string, string->Buffer_Size + 16);
    string->Content[string->Size] = '\0';
}

void Append_Text(const String_Ptr string, size_t max_length, const char *text){
    let size = strnlen(text, max_length);
    let new_size = size + string->Size;
    if(new_size >= string->Buffer_Size)
        // 16-aligned size buffer size
        Extend_Buffer(string, ((new_size + 16) >> 4) << 4);

    strncpy(&string->Content[string->Size], text, size);
    string->Size = new_size;
    string->Content[new_size] = '\0';
}

void Free_String(const String_Ptr string){
    if(string->Managed_Buffer)
        free(string->Content);
    string->Content = NULL;
    string->Size = 0;
    string->Buffer_Size = 0;
    string->Managed_Buffer = false;
}

void Move_String(const String_Ptr dest, const String_Ptr src){
    dest->Size = src->Size;
    dest->Buffer_Size = src->Buffer_Size;
    dest->Managed_Buffer = true;
    if(src->Managed_Buffer){
        dest->Content = src->Content;
    } else {
        dest->Content = talloc(char, src->Buffer_Size);
        strncpy(dest->Content, src->Content, src->Buffer_Size);
    }

    src->Size = 0;
    src->Buffer_Size = 0;
    src->Content = NULL;
}
