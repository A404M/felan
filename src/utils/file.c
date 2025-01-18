#include "file.h"

#include "utils/log.h"
#include "utils/memory.h"
#include <stddef.h>
#include <stdio.h>

char *readWholeFile(const char *filePath) {
  FILE *file = fopen(filePath, "r");

  if (!file) {
    printLog("Can't open file '%s'", filePath);
    return NULL;
  }
  fseek(file, 0, SEEK_END);
  const size_t file_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *str = a404m_malloc((file_size + 1) * sizeof(char));

  fread(str, file_size, 1, file);
  str[file_size] = '\0';

  fclose(file);

  return str;
}
