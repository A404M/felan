#include "log.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void _printLogBack(const char *format, const char *file, int line, ...) {
  va_list args;
  va_start(args, end);
  char *errorStr;
  vasprintf(&errorStr, format, args);

  printf("\e[0;31mError: %s at compiler %s:%d\e[0m\n", errorStr, file, line);
  free(errorStr);
}
