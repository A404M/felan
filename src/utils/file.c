#include "file.h"

#include "utils/log.h"
#include "utils/memory.h"
#include <stdio.h>
#include <string.h>

size_t fileCodes_capacity = 0;
char **fileCodes = NULL;
char **fileCodes_names = NULL;
size_t fileCodes_length = 0;

void fileInit() {
  fileCodes_capacity = 0;
  fileCodes = a404m_malloc(fileCodes_capacity * sizeof(*fileCodes));
  fileCodes_names = a404m_malloc(fileCodes_capacity * sizeof(*fileCodes_names));
  fileCodes_length = 0;
}

void fileDelete() {
  fileCodes_capacity = 0;
  for (size_t i = 0; i < fileCodes_length; ++i) {
    free(fileCodes[i]);
    free(fileCodes_names[i]);
  }
  free(fileCodes);
  free(fileCodes_names);
  fileCodes_length = 0;
}

void filePush(const char *filePath, char *code) {
  if (fileCodes_capacity == fileCodes_length) {
    fileCodes_capacity += fileCodes_capacity / 2 + 1;
    fileCodes =
        a404m_realloc(fileCodes, fileCodes_capacity * sizeof(*fileCodes));
    fileCodes_names = a404m_realloc(
        fileCodes_names, fileCodes_capacity * sizeof(*fileCodes_names));
  }
  fileCodes[fileCodes_length] = code;
  fileCodes_names[fileCodes_length] =
      a404m_malloc((strlen(filePath) + 1) * sizeof(**fileCodes_names));
  strcpy(fileCodes_names[fileCodes_length], filePath);
  fileCodes_length += 1;
}

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

  filePush(filePath, str);

  return str;
}
