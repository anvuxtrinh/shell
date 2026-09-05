#pragma once

#include <stdio.h>

typedef void (*vec_free_cb_t)(void *elem);

typedef struct vec {
    void *data;
    size_t esize;
    size_t size;
    size_t cap;
    vec_free_cb_t free_cb;
} vec_t;

int vec_init(vec_t *self, size_t esize, vec_free_cb_t free_cb);
int vec_push(vec_t *self, void *elem);
void* vec_at(vec_t *self, size_t index);
int vec_remove(vec_t *self, size_t index);
int vec_clear(vec_t *self);
int vec_free(vec_t *self);
int vec_shrink_to_fit(vec_t *self);