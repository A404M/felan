#include "string.h"
#include "memory.h"
#include "utils/type.h"

#include <stdint.h>
#include <stdio.h>
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

u64 decimalToU64(char *str_begin, char *str_end, bool *success) {
  u64 result = 0;

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

f128 numberToFloat(char *str_begin, char *str_end, bool *success) {
  f128 left = 0;
  f128 right = 0;
  bool isPastPoint = false;

  while (str_begin < str_end) {
    char c;
    if (isPastPoint) {
      c = *--str_end;
    } else {
      c = *str_begin++;
    }
    if (c >= '0' && c <= '9') {
      if (isPastPoint) {
        right += c - '0';
        right /= 10;
      } else {
        left *= 10;
        left += c - '0';
      }
    } else if (c == '.' && !isPastPoint) {
      isPastPoint = true;
    } else {
      *success = false;
      return 0;
    }
  }

  *success = true;
  return left + right;
}

char *u64ToString(u64 value) {
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
