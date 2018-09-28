#ifndef STATION_STAT_DEFINED
#define STATION_STAT_DEFINED

typedef struct station_stat_t station_stat_t;

#include "timekeeper.h"
#include <pthread.h>
#include <stdbool.h>

#define STATION_STAT_FORWARD 0
#define STATION_STAT_REVERSE 1

struct station_stat_t {
    int num_door_opening[2];
    double total_wait_time[2];
    double min_wait_time[2];
    double max_wait_time[2];
    double last_closed_time[2];
};

void station_stat_init(station_stat_t* station_stat);
void station_stat_open_door(station_stat_t*, double current_time, double duration, bool forward);
#endif
