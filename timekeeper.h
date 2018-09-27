#include "math.h"
#include <pthread.h>

#define TIMEKEEPER_DEFINED

typedef struct timekeeper_t {
    pthread_mutex_t lock;
    double time;
} timekeeper_t;

/**
 * Initialises a timekeeper with time = 0
 */
void timekeeper_init(timekeeper_t* timekeeper);

/**
 * Increases the timekeeper by duration and returns the new value.
 *
 * This operation is atomic.
 */
double timekeeper_increase_by(timekeeper_t* timekeeper, double duration, double current_time);
