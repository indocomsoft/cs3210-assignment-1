#include "station_stat.h"

#define max(a, b) (a >= b ? a : b)
#define min(a, b) (a <= b ? a : b)

void station_stat_init(station_stat_t* station_stat)
{
    station_stat->num_door_opening = 0;
    station_stat->total_wait_time = 0;
    station_stat->min_wait_time = 0;
    station_stat->max_wait_time = 0;
    station_stat->last_closed_time = 0;
}

void station_stat_open_door(station_stat_t* station_stat, double current_time, double duration)
{
    double time_taken = current_time - station_stat->last_closed_time;

    if (station_stat->num_door_opening == 0) {
        // Do not count waiting time of first train
    } else {
        station_stat->total_wait_time += time_taken;
        if (station_stat->num_door_opening == 1) {
            station_stat->min_wait_time = time_taken;
            station_stat->max_wait_time = time_taken;
        } else {
            station_stat->min_wait_time = min(station_stat->min_wait_time, time_taken);
            station_stat->max_wait_time = max(station_stat->max_wait_time, time_taken);
        }
    }

    station_stat->last_closed_time = current_time + duration;
    station_stat->num_door_opening++;
}
