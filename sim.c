#include <omp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef LINE_DEFINED
#include "line.h"
#endif

#ifndef STATION_STAT_DEFINED
#include "station_stat.h"
#endif

#ifndef TIMEKEEPER_DEFINED
#include "timekeeper.h"
#endif

#ifndef TRAIN_DEFINED
#include "train.h"
#endif

#define MAX_STATION_NAME_LENGTH 100

typedef struct raw_input_t {
    int num_stations;
    char** station_names;
    int** stations;
    double* popularity;
    line_t* green_line;
    line_t* yellow_line;
    line_t* blue_line;
    int ticks;
    int* num_trains;
    int total_trains;
} input_t;

void read_input(input_t*);
void build_line(line_t*, input_t*);
void print_input(input_t*);
void print_line(line_t*);
void cleanup_input(input_t*);
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

void read_input(input_t* input)
{
    int i, j;

    scanf("%d\n", &(input->num_stations));

    char* str_buf = (char*)malloc(sizeof(char) * MAX_STATION_NAME_LENGTH * input->num_stations);

    // STATION NAMES
    input->station_names = (char**)malloc(sizeof(char*) * input->num_stations);
    for (i = 0; i < input->num_stations; i++) {
        input->station_names[i] = (char*)malloc(sizeof(char) * MAX_STATION_NAME_LENGTH);
    }

    fgets(str_buf, MAX_STATION_NAME_LENGTH * input->num_stations, stdin);
    const char* delim = "\n,";
    char* next;
    i = 0;
    for (next = strtok(str_buf, delim); next != NULL; next = strtok(NULL, delim)) {
        strcpy(input->station_names[i], next);
        i++;
    }

    // STATIONS
    input->stations = (int**)malloc(sizeof(int*) * input->num_stations);
    for (i = 0; i < input->num_stations; i++) {
        input->stations[i] = (int*)malloc(sizeof(int) * input->num_stations);
    }

    for (i = 0; i < input->num_stations; i++) {
        fgets(str_buf, MAX_STATION_NAME_LENGTH * input->num_stations, stdin);
        j = 0;
        for (next = strtok(str_buf, " "); next != NULL; next = strtok(NULL, " ")) {
            input->stations[i][j] = atoi(next);
            j++;
        }
    }

    // POPULARITY
    input->popularity = (double*)malloc(sizeof(int*) * input->num_stations);
    fgets(str_buf, MAX_STATION_NAME_LENGTH * input->num_stations, stdin);
    i = 0;
    for (next = strtok(str_buf, delim); next != NULL; next = strtok(NULL, delim)) {
        sscanf(next, "%lf", &(input->popularity[i]));
        i++;
    }

    // GREEN LINE
    input->green_line = (line_t*)malloc(sizeof(line_t));
    build_line(input->green_line, input);
    input->green_line->id = 'g';

    // YELLOW LINE
    input->yellow_line = (line_t*)malloc(sizeof(line_t));
    build_line(input->yellow_line, input);
    input->yellow_line->id = 'y';

    // BLUE LINE
    input->blue_line = (line_t*)malloc(sizeof(line_t));
    build_line(input->blue_line, input);
    input->blue_line->id = 'b';

    // TICKS
    scanf("%d\n", &(input->ticks));

    // NUM TRAINS
    input->num_trains = (int*)malloc(sizeof(int) * 3);

    fgets(str_buf, MAX_STATION_NAME_LENGTH * input->num_stations, stdin);
    i = 0;
    for (next = strtok(str_buf, delim); next != NULL; next = strtok(NULL, delim)) {
        sscanf(next, "%d", &(input->num_trains[i]));
        i++;
    }
    input->green_line->num_trains = input->num_trains[0];
    input->green_line->start_train_id = 0;
    input->yellow_line->num_trains = input->num_trains[1];
    input->yellow_line->start_train_id = input->num_trains[0];
    input->blue_line->num_trains = input->num_trains[2];
    input->blue_line->start_train_id = input->num_trains[0] + input->num_trains[1];

    input->total_trains = input->blue_line->start_train_id + input->blue_line->num_trains;

    // CLEANUP
    free(str_buf);
}

void cleanup_input(input_t* input)
{
    int i;
    for (i = 0; i < input->num_stations; i++) {
        free(input->station_names[i]);
    }
    free(input->station_names);
    for (i = 0; i < input->num_stations; i++) {
        free(input->stations[i]);
    }
    free(input->stations);
    free(input->popularity);
    free(input->green_line->stations);
    free(input->green_line);
    free(input->yellow_line->stations);
    free(input->yellow_line);
    free(input->blue_line->stations);
    free(input->blue_line);
    free(input->num_trains);
    free(input);
}

void build_line(line_t* line, input_t* input)
{
    int i, j;
    char* str_buf = (char*)malloc(sizeof(char) * MAX_STATION_NAME_LENGTH * input->num_stations);
    line->stations = (int*)malloc(sizeof(int) * input->num_stations);

    fgets(str_buf, MAX_STATION_NAME_LENGTH * input->num_stations, stdin);
    i = 0;
    const char* delim = "\n,";
    char* next;

    for (next = strtok(str_buf, delim); next != NULL; next = strtok(NULL, delim)) {
        j = 0;
        while (j < input->num_stations) {
            if (strcmp(input->station_names[j], next) == 0)
                break;
            j++;
        }
        line->stations[i] = j;
        i++;
    }
    line->num_stations = i;
    free(str_buf);

    line->stats = (station_stat_t*)malloc(sizeof(station_stat_t) * line->num_stations);
    for (i = 0; i < line->num_stations; i++) {
        station_stat_init(&line->stats[i]);
    }
}

void print_input(input_t* input)
{
    int i;
    int j;
    printf("%d\n", input->num_stations);
    for (i = 0; i < input->num_stations; i++) {
        printf("%s ", input->station_names[i]);
    }
    printf("\n");

    for (i = 0; i < input->num_stations; i++) {
        for (j = 0; j < input->num_stations; j++) {
            printf("%d ", input->stations[i][j]);
        }
        printf("\n");
    }

    for (i = 0; i < input->num_stations; i++) {
        printf("%.2lf ", input->popularity[i]);
    }
    printf("\n");

    print_line(input->green_line);
    print_line(input->yellow_line);
    print_line(input->blue_line);

    printf("%d\n", input->ticks);

    for (i = 0; i < 3; i++) {
        printf("%d ", input->num_trains[i]);
    }
    printf("\n");
}

void print_line(line_t* line)
{
    int i;
    for (i = 0; i < line->num_stations; i++) {
        printf("%d ", line->stations[i]);
    }
    printf("\n");
}
