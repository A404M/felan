#pragma once

#include <time.h>

struct timespec time_diff(struct timespec end, struct timespec start);
struct timespec time_add(struct timespec left, struct timespec right);
void time_print(struct timespec time);
