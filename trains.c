#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATION_NAME_LENGTH 100

typedef struct raw_line_t {
    int num_stations;
    int* stations;
} line_t;

typedef struct train_t {
    char line;
    int index;
    bool travelling_forward;
    bool doors_open;
    int from;
    int to;
    int distance;
} train_t;

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
} input_t;

void read_input(input_t*);
void build_line(line_t*, input_t*);
void print_input(input_t*);
void print_line(line_t*);
void cleanup_input(input_t*);

int main()
{
    int i;
    int j;
    input_t* input = (input_t*)malloc(sizeof(input_t));

    read_input(input);
    print_input(input);
    cleanup_input(input);
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

    // YELLOW LINE
    input->yellow_line = (line_t*)malloc(sizeof(line_t));
    build_line(input->yellow_line, input);

    // BLUE LINE
    input->blue_line = (line_t*)malloc(sizeof(line_t));
    build_line(input->blue_line, input);

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
