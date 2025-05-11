#pragma once

#include <time.h>

typedef struct timespec Time;
Time time_diff(Time end, Time start);
Time time_add(Time left, Time right);
void time_print(Time time);
Time get_time();
