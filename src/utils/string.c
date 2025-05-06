#include "string.h"
#include "memory.h"
#include "utils/type.h"

size_t strLength(const char *str) {
  size_t i = 0;
  for (; str[i] != '\0'; ++i)
    ;
  return i;
}

bool strEquals(const char *left, const char *right) {
  size_t i = 0;
  for (; left[i] != '\0' && right[i] != '\0'; ++i) {
    if (left[i] != right[i]) {
      return false;
    }
  }
  return left[i] == right[i];
}

bool strnEquals(const char *left, const char *right, size_t len) {
  for (size_t i = 0; i < len; ++i) {
    if (left[i] != right[i]) {
      return false;
    }
  }
  return true;
}

size_t searchInStringArray(const char *array[], size_t array_size,
                           const char *str, size_t str_size) {
  for (size_t i = 0; i < array_size; ++i) {
    const size_t el_size = strLength(array[i]);
    if (el_size == str_size && strnEquals(array[i], str, str_size)) {
      return i;
    }
  }
  return array_size;
}

u64 decimalToU64(char const *str_begin, char const *str_end, bool *success) {
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

f128 numberToFloat(char const *str_begin, char const *str_end, bool *success) {
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

char *strClone(const char *str) {
  const size_t str_length = strLength(str) + 1;
  char *result = a404m_malloc(str_length * sizeof(*result));
  for (size_t i = 0; i < str_length; ++i) {
    result[i] = str[i];
  }
  return result;
}
