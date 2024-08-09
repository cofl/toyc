#ifndef __SCANNER_H__
#define __SCANNER_H__
#include <stdbool.h>
#include "token.h"

int Open (int fd);
int Open_File (const char *path);
int Close (void);
bool Next_Token (const Token_Ptr token);

#endif
