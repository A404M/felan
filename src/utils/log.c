#include "log.h"

#include "utils/file.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void _printLogBack(const char *format, const char *file, int line, ...) {
  va_list args;
  va_start(args, end);
  char *errorStr;
  vasprintf(&errorStr, format, args);

  printf("\e[0;31mError: %s at compiler %s:%d\e[0m\n", errorStr, file, line);
  free(errorStr);
}

void _printErrorWarningBack(const char *file, int line, char *begin, char *end,
                            bool isError, const char *format, ...) {
  va_list args;
  va_start(args, end);
  char *errorStr;
  vasprintf(&errorStr, format, args);

  size_t file_index = SIZE_MAX;
  for (size_t i = 0; i < fileCodes_length; ++i) {
    char *fileCode_begin = fileCodes[i];
    char *fileCode_end = fileCodes[i] + strlen(fileCodes[i]);
    if (begin >= fileCode_begin && end <= fileCode_end) {
      file_index = i;
      break;
    }
  }

  const char firstColor[] = "\e[0;36m";
  const char *secondColor = isError ? "\e[0;31m" : "\e[0;33m";
  if (isError) {
    fprintf(stderr, "%sError: %s at compiler %s:%d\e[0m\n", firstColor,
            errorStr, file, line);
  } else {
    fprintf(stderr, "%sWarning: %s at compiler %s:%d\e[0m\n", firstColor,
            errorStr, file, line);
  }
  free(errorStr);

  if (file_index == SIZE_MAX) {
    return;
  }

  size_t file_line = 1;
  char *file_line_begin = fileCodes[file_index];
  char *file_line_end = fileCodes[file_index];
  for (char *iter = fileCodes[file_index]; *iter != '\0'; ++iter) {
    if (*iter == '\n') {
      if (iter <= begin) {
        file_line_begin = iter + 1;
      }
      if (iter >= end && file_line_end == fileCodes[file_index]) {
        file_line_end = iter;
      }
      if (iter <= end) {
        ++file_line;
      }
    }
  }

  fprintf(stderr, "\e%sAt %s:%ld\n", firstColor, fileCodes_names[file_index],
          file_line);

  for (char *iter = file_line_begin; iter < file_line_end; ++iter) {
    if (iter == begin) {
      fprintf(stderr, "%s", secondColor);
    } else if (iter == end) {
      fprintf(stderr, "%s", firstColor);
    }
    fprintf(stderr, "%c", *iter);
  }
  fprintf(stderr, "\e[0m\n");
}
