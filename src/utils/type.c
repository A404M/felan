#include "type.h"

static_assert(sizeof(bool) == (8 / 8), "Bad size");

static_assert(sizeof(i8) == (8 / 8), "Bad size");
static_assert(sizeof(i16) == (16 / 8), "Bad size");
static_assert(sizeof(i32) == (32 / 8), "Bad size");
static_assert(sizeof(i64) == (64 / 8), "Bad size");

static_assert(sizeof(u8) == (8 / 8), "Bad size");
static_assert(sizeof(u16) == (16 / 8), "Bad size");
static_assert(sizeof(u32) == (32 / 8), "Bad size");
static_assert(sizeof(u64) == (64 / 8), "Bad size");

#ifdef FLOAT_16_SUPPORT
static_assert(sizeof(f16) == (16 / 8), "Bad size");
#endif
static_assert(sizeof(f32) == (32 / 8), "Bad size");
static_assert(sizeof(f64) == (64 / 8), "Bad size");
static_assert(sizeof(f128) == (128 / 8), "Bad size");
