#include "string.h"
#include "memory.h"

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

char *u64ToString(uint64_t value) {
  char *str = a404m_malloc(21 * sizeof(*str));
  size_t i = 0;

  do {
    str[i] = (value % 10) + '0';
    value /= 10;
    ++i;
  } while (value != 0);

  for (size_t j = 0; j < i / 2; ++j) {
    char tmp = str[j];
    str[j] = str[i - j - 1];
    str[i - j - 1] = tmp;
  }
  str[i] = '\0';

  return a404m_realloc(str, (i + 1) * sizeof(*str));
}
