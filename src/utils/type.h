#pragma once

#ifdef __FLT16_MIN__
#define FLOAT_16_SUPPORT
#endif

typedef char i8;
typedef short i16;
typedef int i32;
typedef long long i64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

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

#define static_assert _Static_assert
#endif
#endif

