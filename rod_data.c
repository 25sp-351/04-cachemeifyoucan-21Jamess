#include "rod_data.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_rod_data(const char *filename, RodPiece rod_pieces[],
                   int *count_ptr) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "ERROR: Could not open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_INPUT_LINE_LENGTH];
    *count_ptr = 0;

    while (fgets(buffer, sizeof(buffer), file)) {
        if (buffer[0] == '\n')
            continue;

        int length, value, max_cuts = 0;

        int parsed = sscanf(buffer, "%d, %d, %d", &length, &value, &max_cuts);

        if (parsed < 2 || parsed > 3) {
            fprintf(stderr, "ERROR: Invalid line format: %s", buffer);
            exit(EXIT_FAILURE);
        }

        if (length <= 0 || value <= 0) {
            fprintf(stderr, "ERROR: Invalid length/value in line: %s", buffer);
            exit(EXIT_FAILURE);
        }

        rod_pieces[*count_ptr].length   = length;
        rod_pieces[*count_ptr].value    = value;
        rod_pieces[*count_ptr].max_cuts = (parsed == 3) ? max_cuts : 0;
        (*count_ptr)++;
    }

    fclose(file);
}
