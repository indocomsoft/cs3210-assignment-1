#include "sim.h"

timekeeper_t** setup_track_timekeepers(input_t*);
timekeeper_t* setup_station_timekeepers(input_t*);

void run_simulation(input_t*);

int main()
{
    input_t* input = (input_t*)malloc(sizeof(input_t));

    read_input(input);
    print_input(input);

    run_simulation(input);

    cleanup_input(input);

    return 0;
}

void run_simulation(input_t* input)
{
    omp_set_dynamic(0);
    omp_set_num_threads(input->total_trains);
    printf("total trains: %d\n", input->total_trains);

    timekeeper_t** track_timekeepers = setup_track_timekeepers(input);
    timekeeper_t* station_timekeepers = setup_station_timekeepers(input);

    int cur_time = 0;
#pragma omp parallel shared(track_timekeepers, station_timekeepers)
    {
        train_t* train;
        int tid = omp_get_thread_num();

        if (tid >= input->green_line->start_train_id && tid < input->yellow_line->start_train_id) {
            train = build_train(tid, input->green_line);
        } else if (tid >= input->yellow_line->start_train_id && tid < input->blue_line->start_train_id) {
            train = build_train(tid, input->yellow_line);
        } else {
            train = build_train(tid, input->blue_line);
        }
        while (cur_time < input->ticks) {
            if ((int)train->spawn_time == cur_time) {
                train->next_state = OPEN_DOOR;
                train->next_door_open_duration = input->popularity[train->station_id] * (rand() % 10 + 1);
                train->next_state_time = timekeeper_increase_by(&station_timekeepers[train->station_id], train->next_door_open_duration, cur_time);
            }

            // while (train->next_state_time <= cur_time) {
            //     if (train->next_state == OPEN_DOOR) {
            //         // station_stat_open_door
            //     }
            // }

#pragma omp barrier

#pragma omp single
            {
                cur_time++;
            }
        }
#pragma omp critical
        {
            if (tid < 10) {
                printf("tid: %d\n", tid);
                printf("cur_time: %d, ", cur_time);
                printf("train name: %s, ", train->name);
                if (train->travelling_forward) {
                    printf("forward: true, ");
                } else {
                    printf("forward: false, ");
                }
                printf("train position on line: %d, ", train->line_station_id);
                printf("current station: %s, ", input->station_names[train->station_id]);
                printf("next state time: %lf, ", train->next_state_time);
                printf("%lf\n", train->spawn_time);
            }
        }
    }
}

timekeeper_t** setup_track_timekeepers(input_t* input)
{
    int i, j;

    timekeeper_t** timekeepers = (timekeeper_t**)malloc(sizeof(timekeeper_t*) * input->num_stations);
    for (i = 0; i < input->num_stations; i++) {
        timekeepers[i] = (timekeeper_t*)malloc(sizeof(timekeeper_t) * input->num_stations);
        for (j = 0; j < input->num_stations; j++) {
            timekeeper_init(&timekeepers[i][j]);
        }
    }

    return timekeepers;
}

timekeeper_t* setup_station_timekeepers(input_t* input)
{
    int i;
    timekeeper_t* timekeepers = (timekeeper_t*)malloc(sizeof(timekeeper_t) * input->num_stations);
    for (i = 0; i < input->num_stations; i++) {
        timekeeper_init(&timekeepers[i]);
    }
    return timekeepers;
}
