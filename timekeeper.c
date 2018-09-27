#include "timekeeper.h"

#include <pthread.h>

void timekeeper_init(timekeeper_t* timekeeper)
{
    pthread_mutex_init(&timekeeper->lock, NULL);
    timekeeper->time = 0;
}

double timekeeper_increase_by(timekeeper_t* timekeeper, double duration, double current_time)
{
    pthread_mutex_lock(&timekeeper->lock);
    double start = fmax(current_time, timekeeper->time);
    timekeeper->time = start + duration;
    pthread_mutex_unlock(&timekeeper->lock);
    return start;
}
