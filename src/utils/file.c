#include "file.h"

#include "utils/log.h"
#include "utils/memory.h"
#include "utils/string.h"
#include <stdio.h>

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
  size_t filePath_length = strLength(filePath);
  fileCodes[fileCodes_length] = code;
  fileCodes_names[fileCodes_length] =
      a404m_malloc((filePath_length + 1) * sizeof(**fileCodes_names));
  for (size_t i = 0; i < filePath_length + 1; ++i) {
    fileCodes_names[fileCodes_length][i] = filePath[i];
  }
  fileCodes_length += 1;
}

char *readWholeFile(const char *filePath) {
  const size_t index = getFileIndex(filePath);
  if (index != fileCodes_length) {
    return fileCodes[index];
  }
  FILE *file = fopen(filePath, "r");

  if (!file) {
    printLog("Can't open file '%s'", filePath);
    return NULL;
  }
  fseek(file, 0, SEEK_END);
  const size_t file_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *str = a404m_malloc((file_size + 1) * sizeof(*str));

  fread(str, file_size, 1, file);
  str[file_size] = '\0';

  fclose(file);

  filePush(filePath, str);

  return str;
}

size_t getFileIndex(const char *filePath) {
  for (size_t i = 0; i < fileCodes_length; ++i) {
    const char *str = fileCodes_names[i];
    if (strEquals(filePath, str)) {
      return i;
    }
  }

  return fileCodes_length;
}

char *joinToPathOf(const char *original, const char *file) {
  size_t result_size = 0;
  for (size_t i = 0; original[i] != '\0'; ++i) {
    if (original[i] == '/') {
      result_size = i;
    }
  }
  char *result =
      a404m_malloc((result_size + 1 + strLength(file) + 1) * sizeof(*result));

  for (size_t i = 0; i < result_size; ++i) {
    result[i] = original[i];
  }
  result[result_size++] = '/';

  for (size_t i = 0; file[i] != '\0'; ++i) {
    if (result_size != 0 && result[result_size - 1] == '/' &&
        file[i + 1] == '.') {
      if (file[i + 2] == '/') {
        i += 2;
        continue;
      } else if (file[i + 2] == '.' && file[i + 3] == '/') {
        bool found = false;
        for (size_t j = i - 2; j != -1ULL; --j) {
          if (result[j] == '/') {
            result_size = j;
            found = true;
            break;
          }
        }
        if (!found) {
          result_size = 0;
          result[result_size++] = '.';
          result[result_size++] = '.';
          result[result_size++] = '/';
        }
        continue;
      }
    }
    result[result_size++] = file[i];
  }

  result[result_size++] = '\0';
  result = a404m_realloc(result, result_size);

  return result;
}
