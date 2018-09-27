typedef struct raw_line_t {
    int num_stations;
    int* stations;
    char id;
    int num_trains;
    int start_train_id;
} line_t;

#define LINE_T_DEFINED