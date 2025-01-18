#include "memory.h"

#include <stdlib.h>
#include <malloc.h>

void *a404m_malloc(size_t size) {
  if (size == 0) {
    return NULL;
  } else {
    return malloc(size);
  }
}

void *a404m_realloc(void *restrict pointer, size_t size) {
  if (size == 0) {
    free(pointer);
    return NULL;
  } else if (pointer != NULL) {
    return realloc(pointer, size);
  } else {
    return malloc(size);
  }
}

size_t a404m_malloc_usable_size(void *pointer) {
  if (pointer == NULL) {
    return 0;
  } else {
    return malloc_usable_size(pointer);
  }
}
