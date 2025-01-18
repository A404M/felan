#include "string.h"

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
