#define OPEN_DOOR 0
#define CLOSE_DOOR 1
#define DEPART 2
#define ARRIVE 3

#ifndef LINE_T_DEFINED
#include "line.h"
#endif

#include <stdbool.h>

typedef struct train_t {
    line_t* line;
    int train_id;
    bool travelling_forward;
    int next_state;
    double next_state_time;
    double next_door_open_duration;
    int station_id;
} train_t;
