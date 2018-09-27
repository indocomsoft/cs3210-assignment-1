#ifndef STATION_STAT_DEFINED
#define STATION_STAT_DEFINED

typedef struct station_stat_t station_stat_t;

#include "timekeeper.h"
#include <pthread.h>

struct station_stat_t {
    int num_door_opening;
    double total_wait_time;
    double min_wait_time;
    double max_wait_time;
    double last_closed_time;
};

void station_stat_init(station_stat_t* station_stat);
void station_stat_open_door(station_stat_t*, double current_time, double duration);
#endif