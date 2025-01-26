#include "string.h"

#include <stdint.h>
#include <string.h>

size_t searchInStringArray(const char *array[], size_t array_size,
                           const char *str, size_t str_size) {
  for (size_t i = 0; i < array_size; ++i) {
    const size_t el_size = strlen(array[i]);
    if (el_size == str_size && strncmp(array[i], str, str_size) == 0) {
      return i;
    }
  }
  return array_size;
}

uint64_t decimalToU64(char *str_begin, char *str_end, bool *success) {
  uint64_t result = 0;

  while (str_begin < str_end) {
    if (*str_begin < '0' || *str_begin > '9') {
      *success = false;
      return 0;
    }
    result *= 10;
    result += *str_begin - '0';
    str_begin += 1;
  }

  *success = true;
  return result;
}
