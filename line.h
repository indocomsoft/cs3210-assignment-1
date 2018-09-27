typedef struct raw_line_t {
    int num_stations;
    int* stations;
    char id;
} line_t;

#define LINE_T_DEFINED