#pragma once

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>

struct hashmap_node {
    void *key;
    size_t ksize;
    void *value;
    struct hashmap_node *next;
};

typedef struct hashmap {
    int size;
    int cap;
    struct hashmap_node **buckets;
} hashmap_t;

int hashmap_init(hashmap_t *self);
int hashmap_put(hashmap_t *self, const void *key, size_t ksize, void *value);
void *hashmap_get(hashmap_t *self, const void *key, size_t ksize);
int hashmap_remove(hashmap_t *self, const void *key, size_t ksize);
bool hashmap_contains(hashmap_t *self, const void *key, size_t ksize);
int hashmap_rehash(hashmap_t *self);
