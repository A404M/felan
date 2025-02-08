#include "file.h"

#include "utils/log.h"
#include "utils/memory.h"
#include <stddef.h>
#include <stdio.h>

size_t fileCodes_capacity = 0;
char **fileCodes = NULL;
const char **fileCodes_names = 0;
size_t fileCodes_length = 0;

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

  if (fileCodes_capacity == fileCodes_length) {
    fileCodes_capacity += fileCodes_capacity / 2 + 1;
    fileCodes =
        a404m_realloc(fileCodes, fileCodes_capacity * sizeof(*fileCodes));
    fileCodes_names = a404m_realloc(
        fileCodes_names, fileCodes_capacity * sizeof(*fileCodes_names));
  }
  fileCodes[fileCodes_length] = str;
  fileCodes_names[fileCodes_length] = filePath;
  fileCodes_length += 1;

  return str;
}
