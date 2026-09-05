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
typedef int32_t   b32;
typedef int32_t   i32;
typedef uint32_t  u32;
typedef uint64_t  u64;
typedef float     f32;
typedef double    f64;

#endif
