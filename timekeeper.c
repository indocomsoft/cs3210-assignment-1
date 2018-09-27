#include "timekeeper.h"

#include <pthread.h>

void timekeeper_init(timekeeper_t* timekeeper)
{
    pthread_mutex_init(&timekeeper->lock, NULL);
    timekeeper->time = 0;
}

int timekeeper_increase_by(timekeeper_t* timekeeper, int duration)
{
    int result;
    pthread_mutex_lock(&timekeeper->lock);
    timekeeper->time += duration;
    result = timekeeper->time;
    pthread_mutex_unlock(&timekeeper->lock);
    return result;
}
