#pragma once

#include <stdio.h>
#include <stddef.h>

#include "type_ops.h"

typedef struct vec {
    void *data;
    size_t esize;
    size_t size;
    size_t cap;
    const data_ops_t *ops;
} vec_t;

int vec_init(vec_t *self, size_t esize, const data_ops_t *ops);
int vec_deinit(vec_t *self);
int vec_push(vec_t *self, const void *elem);
const void* vec_at(const vec_t *self, size_t index);
int vec_remove(vec_t *self, size_t index);
int vec_clear(vec_t *self);
int vec_shrink_to_fit(vec_t *self);