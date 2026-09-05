#include <stdlib.h>

#include <string.h>

#include "data_structure/hashmap.h"

#ifdef UNIT_TEST
#define STATIC
#define STATIC_INLINE
#else
#define STATIC static
#define STATIC_INLINE static inline
#endif

#define INITIAL_CAPACITY 8
#define MAX_LOAD_FACTOR_NUM 3
#define MAX_LOAD_FACTOR_DEN 4

STATIC_INLINE bool should_rehash(hashmap_t *self) {
    return self->size * MAX_LOAD_FACTOR_DEN >= self->cap * MAX_LOAD_FACTOR_NUM;
}

STATIC_INLINE struct hashmap_node *find_node(struct hashmap_node *head, any_t key, size_t ksize) {
    while (head) {
        if (head->ksize == ksize && memcmp(head->key, key, ksize) == 0) {
            return head;
        }
        head = head->next;
    }
    return NULL;
}

STATIC_INLINE void free_node(struct hashmap_node *node) {
    if (node) {
        free(node);
    }
}

STATIC_INLINE unsigned int hashmap_hash(any_t key, size_t ksize) {
    unsigned long hash = 5381;

    unsigned char *str = (unsigned char *)key;
    for (size_t i = 0; i < ksize; i++) {
        hash = ((hash << 5) + hash) + str[i];
    }
    return (unsigned int)hash;
}

STATIC struct hashmap_node *create_node(any_t key, size_t ksize, any_t value) {
    if (key == NULL || ksize == 0) { return NULL; }
    struct hashmap_node *node = malloc(sizeof(struct hashmap_node));
    if (node == NULL) {
        return NULL;
    }
    node->key = key;
    node->ksize = ksize;
    node->value = value;
    node->next = NULL;
    return node;
}

int hashmap_init(hashmap_t *self) {
    if (self == NULL) { return -EINVAL; }
    
    self->buckets = NULL;
    self->size = 0;
    self->cap = 0;
    return 0;
}

int hashmap_put(hashmap_t *self, any_t key, size_t ksize, any_t value) {
    if(self == NULL || key == NULL || ksize == 0) {
        return -EINVAL;
    }

    if(self->cap == 0){
        self->cap = INITIAL_CAPACITY;
        self->buckets = calloc(self->cap, sizeof(struct hashmap_node *));
        if (self->buckets == NULL) {
            return -ENOMEM;
        }
    }

    if (should_rehash(self)) {
        (void) hashmap_rehash(self);
    }

    unsigned int idx = hashmap_hash(key, ksize) % self->cap;
    struct hashmap_node *cur = self->buckets[idx];

    // Update value if key already exists
    struct hashmap_node *existing_node = find_node(cur, key, ksize);
    if (existing_node) {
        existing_node->value = value;
        return 0;
    }

    // Create a new node
    struct hashmap_node *new_node = create_node(key, ksize, value);
    if (new_node == NULL) {
        return -ENOMEM;
    }
    new_node->next = self->buckets[idx];
    self->buckets[idx] = new_node;
    self->size++;
    return 0;
}

any_t hashmap_get(hashmap_t *self, any_t key, size_t ksize) {
    if(self == NULL || key == NULL || ksize == 0) {
        return NULL;
    }

    if(self->cap == 0 || self->buckets == NULL) {
        return NULL;
    }

    unsigned int idx = hashmap_hash(key, ksize) % self->cap;
    struct hashmap_node *cur = self->buckets[idx];

    struct hashmap_node *node = find_node(cur, key, ksize);
    if (node) {
        return node->value;
    }
    return NULL; // Key not found
}

void hashmap_free(hashmap_t *self) {
    if (self == NULL) {
        return;
    }

    for (int i = 0; i < self->cap; i++) {
        struct hashmap_node *node = self->buckets[i];
        while (node) {
            struct hashmap_node *next = node->next;
            free_node(node);
            node = next;
        }
    }
    free(self->buckets);
    self->size = 0;
    self->cap = 0;
    self->buckets = NULL;
}

int hashmap_rehash(hashmap_t *self) {
    if(self == NULL || self->cap == 0 || self->buckets == NULL) {
        return -EINVAL;
    }

    int old_cap = self->cap;
    struct hashmap_node **old_buckets = self->buckets;

    self->cap *= 2;
    self->buckets = calloc(self->cap, sizeof(struct hashmap_node *));
    if (self->buckets == NULL) {
        self->cap = old_cap;
        self->buckets = old_buckets;
        return -ENOMEM;
    }

    for(int i = 0; i < old_cap; ++i){
        struct hashmap_node *node = old_buckets[i];
        while(node){
            unsigned int idx = hashmap_hash(node->key, node->ksize) % self->cap;
            struct hashmap_node *next_node = node->next;
            node->next = self->buckets[idx];
            self->buckets[idx] = node;
            node = next_node;
        }
    }

    free(old_buckets);
    return 0;
}

int hashmap_remove(hashmap_t *self, any_t key, size_t ksize) {
    if(self == NULL || key == NULL || ksize == 0) {
        return -EINVAL;
    }

    if(self->cap == 0 || self->buckets == NULL) {
        return -ENOENT;
    }

    unsigned int idx = hashmap_hash(key, ksize) % self->cap;
    struct hashmap_node *cur = self->buckets[idx];
    struct hashmap_node *prev = NULL;

    while (cur) {
        if (cur->ksize == ksize && memcmp(cur->key, key, ksize) == 0) {
            if (prev) {
                prev->next = cur->next;
            } else {
                self->buckets[idx] = cur->next;
            }
            free_node(cur);
            self->size--;
            return 0;
        }
        prev = cur;
        cur = cur->next;
    }
    return -ENOENT;
}

bool hashmap_contains(hashmap_t *self, any_t key, size_t ksize) {
    if (self == NULL || key == NULL || ksize == 0) {
        return false;
    }

    if (self->cap == 0 || self->buckets == NULL) {
        return false;
    }

    unsigned int idx = hashmap_hash(key, ksize) % self->cap;
    struct hashmap_node *cur = self->buckets[idx];

    while (cur) {
        if (cur->ksize == ksize && memcmp(cur->key, key, ksize) == 0) {
            return true;
        }
        cur = cur->next;
    }

    return false;
}