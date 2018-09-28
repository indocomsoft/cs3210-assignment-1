#include "station_stat.h"

#define max(a, b) (a >= b ? a : b)
#define min(a, b) (a <= b ? a : b)

void station_stat_init(station_stat_t* station_stat)
{
    for (int i = 0; i < 2; i++) {
        station_stat->num_door_opening[i] = 0;
        station_stat->total_wait_time[i] = 0;
        station_stat->min_wait_time[i] = 0;
        station_stat->max_wait_time[i] = 0;
        station_stat->last_closed_time[i] = 0;
    }
}

void station_stat_open_door(station_stat_t* station_stat, double current_time, double duration, bool forward)
{
    int idx = forward ? STATION_STAT_FORWARD : STATION_STAT_REVERSE;
    double time_taken = current_time - station_stat->last_closed_time[idx];

    if (station_stat->num_door_opening[idx] == 0) {
        // Do not count waiting time of first train
    } else {
        station_stat->total_wait_time[idx] += time_taken;
        if (station_stat->num_door_opening[idx] == 1) {
            station_stat->min_wait_time[idx] = time_taken;
            station_stat->max_wait_time[idx] = time_taken;
        } else {
            station_stat->min_wait_time[idx] = min(station_stat->min_wait_time[idx], time_taken);
            station_stat->max_wait_time[idx] = max(station_stat->max_wait_time[idx], time_taken);
        }
    }

    station_stat->last_closed_time[idx] = current_time + duration;
    station_stat->num_door_opening[idx]++;
}
