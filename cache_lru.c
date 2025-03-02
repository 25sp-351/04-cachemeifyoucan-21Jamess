#include <stdio.h>
#include <stdlib.h>

#include "cache_policy.h"

typedef struct Node {
    int key;
    int value;
    struct Node *prev;
    struct Node *next;
} Node;

struct Cache {
    int capacity;
    int size;
    Node *head;
    Node *tail;
    Node **hash_map;
};

static void remove_node(Cache *cache, Node *node) {
    if (node->prev)
        node->prev->next = node->next;
    else
        cache->head = node->next;
    if (node->next)
        node->next->prev = node->prev;
    else
        cache->tail = node->prev;
    cache->size--;
}

static void add_to_head(Cache *cache, Node *node) {
    node->prev = NULL;
    node->next = cache->head;
    if (cache->head)
        cache->head->prev = node;
    cache->head = node;
    if (!cache->tail)
        cache->tail = node;
    cache->size++;
}

Cache *cache_create(int capacity) {
    Cache *cache = malloc(sizeof(Cache));
    if (!cache)
        return NULL;
    cache->capacity = capacity;
    cache->size     = 0;
    cache->head     = NULL;
    cache->tail     = NULL;
    cache->hash_map = calloc(capacity, sizeof(Node *));
    if (!cache->hash_map) {
        free(cache);
        return NULL;
    }
    return cache;
}

void cache_destroy(Cache *cache) {
    Node *current = cache->head;
    while (current) {
        Node *next = current->next;
        free(current);
        current = next;
    }
    free(cache->hash_map);
    free(cache);
}

int cache_get(Cache *cache, int key) {
    int index  = key % cache->capacity;
    Node *node = cache->hash_map[index];
    if (!node || node->key != key)
        return -1;
    remove_node(cache, node);
    add_to_head(cache, node);
    return node->value;
}

void cache_put(Cache *cache, int key, int value) {
    int index  = key % cache->capacity;
    Node *node = cache->hash_map[index];
    if (node && node->key == key) {
        node->value = value;
        remove_node(cache, node);
        add_to_head(cache, node);
        return;
    }
    if (cache->size == cache->capacity) {
        Node *lru                                   = cache->tail;
        cache->hash_map[lru->key % cache->capacity] = NULL;
        remove_node(cache, lru);
        free(lru);
    }
    Node *new_node = malloc(sizeof(Node));
    if (!new_node)
        return;
    new_node->key          = key;
    new_node->value        = value;
    cache->hash_map[index] = new_node;
    add_to_head(cache, new_node);
}
