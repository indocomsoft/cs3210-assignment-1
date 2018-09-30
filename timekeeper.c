#include "timekeeper.h"

void timekeeper_init(timekeeper_t* timekeeper)
{
    timekeeper->time = 0;
}

int timekeeper_increase_by(timekeeper_t* timekeeper, int duration, int current_time)
{
    int start;
#pragma omp critical
    {
        start = max(current_time, timekeeper->time);
        timekeeper->time = start + duration;
    }
    return start;
}
