#include "common.h"
#define LIT(...) __VA_ARGS__

typedef struct {
    int a, b, c;
} demo;
demo a[] = {
    [0] = (demo){ .a = 1, .b= 2, .c= 3 },
    [1] = (demo){ .a = 1, .b= 2, .c= 3 }
};
let b = arr_sizeof(a);
let c = (int)arr_sizeof((int[]){ 1, 2, 3, 4, 5 });

void parse(){
    
}
