#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include "data_structure/vec.h"

#define VECTOR_INITIAL_CAPACITY 4
#define VECTOR_GROWTH_FACTOR 2
#define VECTOR_SHRINK_THRESHOLD 2

static inline bool should_grow(const vec_t *self) {
    return self->cap == 0 || self->size >= self->cap;
}

static inline bool should_shrink(const vec_t *self) {
    return self->cap > VECTOR_INITIAL_CAPACITY && self->size < self->cap / VECTOR_SHRINK_THRESHOLD;
}

static inline void* get_element_ptr(const vec_t *self, size_t index) {
    return (char *)self->data + (index * self->esize);
}

static int vec_grow_capacity(vec_t *self) {
    size_t new_cap = self->cap == 0 ? VECTOR_INITIAL_CAPACITY : self->cap * VECTOR_GROWTH_FACTOR;
    void *new_data = realloc(self->data, new_cap * self->esize);
    if(new_data == NULL) { return ENOMEM; }
    self->data = new_data;
    self->cap = new_cap;
    return 0;
}

static int vec_shrink_capacity(vec_t *self) {
    size_t new_cap = self->cap / VECTOR_GROWTH_FACTOR;
    new_cap = new_cap < VECTOR_INITIAL_CAPACITY ? VECTOR_INITIAL_CAPACITY : new_cap;

    void *new_data = realloc(self->data, new_cap * self->esize);
    if(new_data == NULL) { return ENOMEM; }
    self->data = new_data;
    self->cap = new_cap;
    return 0;
}

int vec_init(vec_t *self, size_t esize, const data_ops_t *ops) {
    if(self == NULL || esize == 0) { return EINVAL; }

    self->data = NULL;
    self->esize = esize;
    self->size = 0;
    self->cap = 0;
    self->ops = ops;
    return 0;
}

int vec_push(vec_t *self, const void *item) {
    if(self == NULL || item == NULL) { return EINVAL; }

    if(should_grow(self)) {
        int ret = vec_grow_capacity(self);
        if(ret != 0) { return ret; }
    }

    void *dest_ptr = get_element_ptr(self, self->size);
    if(self->ops != NULL && self->ops->clone != NULL) {
        void *cloned_elem = self->ops->clone(item);
        if(cloned_elem == NULL) { return ENOMEM; }
        memcpy(dest_ptr, cloned_elem, self->esize);
        free(cloned_elem);
    } else {
        memcpy(dest_ptr, item, self->esize);
    }

    self->size++;
    return 0;
}

const void* vec_at(const vec_t *self, size_t index) {
    if(self == NULL) { return NULL; }
    if(index >= self->size) { return NULL; }

    return get_element_ptr(self, index);
}

int vec_remove(vec_t *self, size_t index) {
    if(self == NULL) { return EINVAL; }
    if(index >= self->size) { return ERANGE; }

    void *elem_ptr = get_element_ptr(self, index);

    if(self->ops != NULL && self->ops->deinit != NULL) {
        self->ops->deinit(elem_ptr);
    }

    if(index < self->size - 1) {
        void *next_elem_ptr = get_element_ptr(self, index + 1);
        memmove(elem_ptr, next_elem_ptr, (self->size - index - 1) * self->esize);
    }

    self->size--;
    
    if(should_shrink(self)) {
        (void) vec_shrink_capacity(self);
    }

    return 0;
}

int vec_clear(vec_t *self) {
    if(self == NULL) { return EINVAL; }

    if(self->data != NULL && self->ops && self->ops->deinit != NULL) {
        for(size_t i = 0; i < self->size; i++) {
            void *elem_ptr = get_element_ptr(self, i);
            self->ops->deinit(elem_ptr);
        }
    }

    self->size = 0;
    return 0;
}

int vec_deinit(vec_t *self) {
    if(self == NULL) { return EINVAL; }

    vec_clear(self);
    if(self->data != NULL) {
        free(self->data);
        self->data = NULL;
    }
    self->size = 0;
    self->cap = 0;
    return 0;
}

int vec_shrink_to_fit(vec_t *self) {
    if(self == NULL) { return EINVAL; }

    if(self->size == 0) {
        if(self->data != NULL) {
            free(self->data);
            self->data = NULL;
        }
        self->cap = 0;
        return 0;
    }

    if(self->size == self->cap) {
        return 0; // No need to shrink
    }

    size_t new_cap = self->size;
    void *new_data = realloc(self->data, new_cap * self->esize);
    if(new_data == NULL) { return ENOMEM; }

    self->data = new_data;
    self->cap = new_cap;
    return 0;
}