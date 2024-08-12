#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdint.h>
#define uint uint32_t
#define talloc(type, size) (typeof(type)*)malloc(sizeof(type) * (size))
#define trealloc(ptr, size) (typeof(ptr))realloc((ptr), sizeof(typeof(*(ptr))) * (size))
#define arr_sizeof(...) (sizeof((__VA_ARGS__)) / sizeof((__VA_ARGS__)[0]))
#define foreach(var, from, array) for(let var = (from); var < (typeof(var)) arr_sizeof(array); var += 1)
#define ridx(array, index) ((array)[arr_sizeof(array) + (index)])
#define loop for(;;)

struct Position {
    uint Line;
    uint Column;
};

#define let __auto_type
#endif
