#include "type.h"

#include <assert.h>

void checkTypes() {
  assert(sizeof(i8) == (8 / 8));
  assert(sizeof(i16) == (16 / 8));
  assert(sizeof(i32) == (32 / 8));
  assert(sizeof(i64) == (64 / 8));

  assert(sizeof(u8) == (8 / 8));
  assert(sizeof(u16) == (16 / 8));
  assert(sizeof(u32) == (32 / 8));
  assert(sizeof(u64) == (64 / 8));

  assert(sizeof(f16) == (16 / 8));
  assert(sizeof(f32) == (32 / 8));
  assert(sizeof(f64) == (64 / 8));
  assert(sizeof(f128) == (128 / 8));
}

