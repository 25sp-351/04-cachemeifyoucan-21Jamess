#ifndef CACHE_POLICY_H
#define CACHE_POLICY_H

#ifdef USE_LRU_POLICY
#include "cache_lru.h"
#elif defined(USE_MRU_POLICY)
#include "cache_mru.h"
#endif

typedef struct Cache Cache;
Cache *cache_create(int capacity);
void cache_destroy(Cache *cache);
int cache_get(Cache *cache, int key);
void cache_put(Cache *cache, int key, int value);

#endif
