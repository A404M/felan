#pragma once

#include <stddef.h>
#include <stdint.h>

size_t searchInStringArray(const char *array[], size_t array_size,
                           const char *str, size_t str_size);

uint64_t decimalToU64(char *str_begin, char *str_end, bool *success);
char* u64ToString(uint64_t value);
