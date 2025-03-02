#include "rod_cut.h"

#include <stdlib.h>
#include <string.h>

int maximize_value(const RodPiece rod_pieces[], int count, int remaining_length,
                   CutState *cut_state, Cache *cache, int cuts[]) {
    if (remaining_length == 0)
        return 0;

    // Generate a unique cache key based on remaining_length and cut_state
    int key = remaining_length;
    for (int i = 0; i < count; i++)
        key = key * 31 + cut_state->remaining_cuts[i];  // Simple hash

    // Check cache
    int cached_value = cache_get(cache, key);
    if (cached_value != -1)
        return cached_value;

    int max_value = 0;

    for (int piece_idx = 0; piece_idx < count; piece_idx++) {
        if (rod_pieces[piece_idx].length > remaining_length)
            continue;

        // Check if max_cuts is enforced and remaining cuts > 0
        if (rod_pieces[piece_idx].max_cuts > 0 &&
            cut_state->remaining_cuts[piece_idx] <= 0) {
            continue;
        }

        // Clone the current cut state
        CutState new_state = *cut_state;
        if (rod_pieces[piece_idx].max_cuts > 0)
            new_state.remaining_cuts[piece_idx]--;

        int temp_cuts[MAX_ROD_TYPES] = {0};
        int current_value =
            rod_pieces[piece_idx].value +
            maximize_value(rod_pieces, count,
                           remaining_length - rod_pieces[piece_idx].length,
                           &new_state, cache, temp_cuts);

        if (current_value > max_value) {
            max_value = current_value;
            for (int i = 0; i < count; i++)
                cuts[i] = temp_cuts[i];
            cuts[piece_idx]++;
        }
    }

    // Update cache
    cache_put(cache, key, max_value);
    return max_value;
}
