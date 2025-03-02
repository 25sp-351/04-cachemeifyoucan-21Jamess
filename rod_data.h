#ifndef ROD_DATA_H
#define ROD_DATA_H

#define MAX_ROD_TYPES 100
#define MAX_INPUT_LINE_LENGTH 256

typedef struct {
    int length;
    int value;
    int max_cuts;
} RodPiece;

void read_rod_data(const char *filename, RodPiece rod_pieces[], int *count_ptr);

#endif
