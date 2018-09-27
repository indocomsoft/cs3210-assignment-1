#ifndef TIMEKEEPER_DEFINED
#include "timekeeper.h"
#endif

#include <pthread.h>

#define STATION_STAT_DEFINED

typedef struct station_stat_t {
    int num_door_opening;
    int total_wait_time;
    int min_wait_time;
    int max_wait_time;
    int last_closed_time;
} station_stat_t;

void station_stat_init(station_stat_t* station_stat);
void station_stat_open_door(station_stat_t*, int current_time, int duration);
