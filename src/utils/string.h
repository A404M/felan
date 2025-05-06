#pragma once

#include "utils/type.h"
#include <stddef.h>
#include <stdint.h>

size_t strLength(const char *str);

bool strEquals(const char *left, const char *right);
bool strnEquals(const char *left, const char *right, size_t len);

size_t searchInStringArray(const char *array[], size_t array_size,
                           const char *str, size_t str_size);

u64 decimalToU64(char const*str_begin, char const*str_end, bool *success);
f128 numberToFloat(char const*str_begin, char const*str_end, bool *success);
char *u64ToString(u64 value);

char *strClone(const char *str);
