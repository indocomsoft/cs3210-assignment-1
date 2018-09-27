#include <omp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"
#include "line.h"
#include "station_stat.h"
#include "timekeeper.h"
#include "train.h"

timekeeper_t** setup_track_timekeepers(input_t*);
timekeeper_t* setup_station_timekeepers(input_t*);
void run_simulation(input_t*);
void print_train_status(int cur_time, double previous_event_time, char** station_names, train_t* train, int next_station_id, char* status);