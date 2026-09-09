#ifndef HELPFUL_H
#define HELPFUL_H

#include <stdint.h>
#include <stddef.h>

// snippets from
// https://nullprogram.com/blog/2023/10/08/

// simple helpful macros
#define countof(a)    (size)(sizeof(a) / sizeof(*(a)))
#define lengthof(s)   (countof(s) - 1)

// signed types
typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;
typedef int32_t   b32;
typedef int32_t   i32;
typedef int64_t   i64;
typedef float     f32;
typedef double    f64;
typedef ptrdiff_t size;

// basic string type
#define s8(s) (s8){(u8 *)s, lengthof(s)}
typedef struct {
    u8  *data;
    size len;
} s8;


#endif
