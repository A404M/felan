#pragma once

#include <stdint.h>

#ifdef __FLT16_MIN__
#define FLOAT_16_SUPPORT
#endif

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#ifdef FLOAT_16_SUPPORT
typedef _Float16 f16;
#endif
typedef float f32;
typedef double f64;
typedef long double f128;

#ifndef __cplusplus
#if (__STDC_VERSION__ < 202000L)
typedef u8 bool;
#define false (bool)0
#define true (bool)1
#endif
#endif

void checkTypes();
