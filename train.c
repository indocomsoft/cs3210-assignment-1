#include "train.h"

train_t* build_train(int tid, line_t* line)
{
    train_t* train = (train_t*)malloc(sizeof(train_t));
    train->line = line;
    train->id = tid - line->start_train_id;
    if (train->id % 2 == 0) {
        train->travelling_forward = true;
        train->station_id = 0;
    } else {
        train->travelling_forward = false;
        train->station_id = line->num_stations - 1;
    }

    train->spawn_time = tid / 2;
    train->name = (char*)malloc(sizeof(char) * MAX_TRAIN_NAME_LENGTH);
    sprintf(train->name, "%c%d", train->line->id, train->id);

    return train;
}