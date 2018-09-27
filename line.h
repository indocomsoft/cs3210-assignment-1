#ifndef STATION_STAT_DEFINED
#include "station_stat.h"
#endif

#define LINE_DEFINED

typedef struct raw_line_t {
    int num_stations;
    int* stations;
    station_stat_t* stats;
    char id;
    int num_trains;
    int start_train_id;
} line_t;
