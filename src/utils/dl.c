#include "dl.h"
#include "utils/memory.h"
#include "utils/string.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

void **dynamicLibraries;
char **dynamicLibraries_str;
size_t dynamicLibraries_length;
size_t dynamicLibraries_capacity;

void dynamicLibraryInit() {
  dynamicLibraries = a404m_malloc(0);
  dynamicLibraries_str = a404m_malloc(0);
  dynamicLibraries_length = 0;
  dynamicLibraries_capacity = 0;
}

void dynamicLibraryDestroy() {
  for (size_t i = 0; i < dynamicLibraries_length; ++i) {
    dlclose(dynamicLibraries[i]);
    free(dynamicLibraries_str[i]);
  }
  free(dynamicLibraries);
  free(dynamicLibraries_str);
}

void *dynamicLibraryOpen(char *file) {
  for (size_t i = 0; i < dynamicLibraries_length; ++i) {
    if (strEquals(file, dynamicLibraries_str[i])) {
      return dynamicLibraries[i];
    }
  }

  void *dl = dlopen(file, RTLD_LAZY);
  if (dl == NULL) {
    fputs(dlerror(), stderr);
    return NULL;
  }

  if (dynamicLibraries_capacity == dynamicLibraries_length) {
    dynamicLibraries_capacity += dynamicLibraries_capacity / 2 + 1;
    dynamicLibraries_str =
        a404m_realloc(dynamicLibraries_str, sizeof(*dynamicLibraries_str) *
                                                dynamicLibraries_capacity);
    dynamicLibraries =
        a404m_realloc(dynamicLibraries,
                      sizeof(*dynamicLibraries) * dynamicLibraries_capacity);
  }
  dynamicLibraries[dynamicLibraries_length] = dl;

  const size_t file_length = strLength(file);
  dynamicLibraries_str[dynamicLibraries_length] =
      a404m_malloc(sizeof(**dynamicLibraries) * (file_length + 1));

  for (size_t i = 0; i < file_length; ++i) {
    dynamicLibraries_str[dynamicLibraries_length][i] = file[i];
  }
  dynamicLibraries_str[dynamicLibraries_length][file_length] = '\0';

  dynamicLibraries_length += 1;

  return dl;
}
