#ifndef ROD_CUT_H
#define ROD_CUT_H

#include "cache_policy.h"
#include "rod_data.h"

typedef struct {
    int remaining_cuts[MAX_ROD_TYPES];
} CutState;

int maximize_value(const RodPiece rod_pieces[], int count, int remaining_length,
                   CutState *cut_state, Cache *cache, int cuts[]);

#endif
