#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cache_policy.h"
#include "rod_cut.h"
#include "rod_data.h"

// Function declaration for maximize_value
int maximize_value(RodPiece rod_pieces[], int rod_type_count, int rod_length,
                   CutState *cut_state, Cache *cache, int cuts[]);

#define CACHE_CAPACITY 100
int parse_rod_length(const char *arg) {
    while (isspace(*arg))
        arg++;

    char *end = (char *)arg + strlen(arg) - 1;
    while (end > arg && isspace(*end))
        end--;
    *(end + 1) = '\0';

    // Convert to integer
    char *endptr;
    long length = strtol(arg, &endptr, 10);
    if (*endptr != '\0' || length <= 0) {
        fprintf(stderr, "ERROR: Invalid rod length: %s\n", arg);
        exit(EXIT_FAILURE);
    }
    return (int)length;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "USAGE: %s <rod_data_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    RodPiece rod_pieces[MAX_ROD_TYPES];
    int rod_type_count;
    read_rod_data(argv[1], rod_pieces, &rod_type_count);

    char buffer[MAX_INPUT_LINE_LENGTH];
    while (fgets(buffer, sizeof(buffer), stdin)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        int rod_length                = parse_rod_length(buffer);

        // Initialize cut state (max_cuts defaults to 0 = no limit)
        CutState cut_state;
        for (int i = 0; i < rod_type_count; i++)
            cut_state.remaining_cuts[i] = rod_pieces[i].max_cuts;

        Cache *cache            = cache_create(CACHE_CAPACITY);
        int cuts[MAX_ROD_TYPES] = {0};
        int total_value = maximize_value(rod_pieces, rod_type_count, rod_length,
                                         &cut_state, cache, cuts);
        cache_destroy(cache);

        // Output results
        printf("The Best Cuts for Length %d:\n", rod_length);
        int used_length = 0;
        for (int i = 0; i < rod_type_count; i++) {
            if (cuts[i] > 0) {
                printf("%d x %d cm (Value: %d)\n", cuts[i],
                       rod_pieces[i].length, cuts[i] * rod_pieces[i].value);
                used_length += cuts[i] * rod_pieces[i].length;
            }
        }
        printf("Remaining: %d cm\n", rod_length - used_length);
        printf("Total Value: %d\n\n", total_value);
    }

    return EXIT_SUCCESS;
}
