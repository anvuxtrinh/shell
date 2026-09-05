#pragma once

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>

typedef void *any_t;

struct hashmap_node {
    any_t key;
    size_t ksize;
    any_t value;
    struct hashmap_node *next;
};

typedef struct hashmap {
    int size;
    int cap;
    struct hashmap_node **buckets;
} hashmap_t;

int hashmap_init(hashmap_t *self);
int hashmap_put(hashmap_t *self, any_t key, size_t ksize, any_t value);
any_t hashmap_get(hashmap_t *self, any_t key, size_t ksize);
void hashmap_free(hashmap_t *self);
int hashmap_rehash(hashmap_t *self);
int hashmap_remove(hashmap_t *self, any_t key, size_t ksize);
bool hashmap_contains(hashmap_t *self, any_t key, size_t ksize);
