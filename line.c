#include "line.h"

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

void print_line(line_t* line)
{
    int i;
    for (i = 0; i < line->num_stations; i++) {
        printf("%d ", line->stations[i]);
    }
    printf("\n");
}
