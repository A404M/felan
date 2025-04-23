#pragma once

#include "utils/type.h"
#include <stddef.h>
#include <stdint.h>

size_t searchInStringArray(const char *array[], size_t array_size,
                           const char *str, size_t str_size);

u64 decimalToU64(char *str_begin, char *str_end, bool *success);
f128 numberToFloat(char *str_begin, char *str_end, bool *success);
char* u64ToString(u64 value);

char *strClone(const char *str);
