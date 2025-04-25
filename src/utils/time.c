#include "time.h"

#include <stdio.h>

#ifdef PRINT_STATISTICS
struct timespec time_diff(struct timespec end, struct timespec start) {
  struct timespec temp;
  if ((end.tv_nsec - start.tv_nsec) < 0) {
    temp.tv_sec = end.tv_sec - start.tv_sec - 1;
    temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
  } else {
    temp.tv_sec = end.tv_sec - start.tv_sec;
    temp.tv_nsec = end.tv_nsec - start.tv_nsec;
  }
  return temp;
}

struct timespec time_add(struct timespec left, struct timespec right) {
  struct timespec result;
  result.tv_nsec = left.tv_nsec + right.tv_nsec;
  result.tv_sec = (left.tv_sec + right.tv_sec) + result.tv_nsec / 1000000000;
  result.tv_nsec %= 1000000000;
  return result;
}

void time_print(struct timespec time) {
  printf("%02ld:%02ld.%06ldus", time.tv_sec / 60, time.tv_sec % 60,
         time.tv_nsec / 1000);
}
#endif


