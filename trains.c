#include <stdio.h>

typedef struct map_t {
    int numStations;
    char** stationNames;
    int** stations;
    double* popularity;

} map_t;

int** buildStations(int size);

int main()
{
    int i;
    int j;
    int numStations;
    int** stations;
    scanf("%d\n", &numStations);

    stations = buildStations(numStations);

    for (i = 0; i < stations; i++) {
        for (j = 0; j < stations; j++) {
        }
    }
}

int** buildStations(int size)
{
    int i;

    int** stations = (int**)malloc(sizeof(int*) * size);

    for (int i = 0; i < size; i++) {
        stations[i] = (int*)malloc(sizeof(int) * size);
    }

    return stations;
}
