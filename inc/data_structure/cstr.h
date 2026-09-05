#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct cstr {
    char *data;
    size_t len;
    size_t cap;
} cstr_t;

void cstr_init(cstr_t *self);
int cstr_appendn(cstr_t *self, const char *str, size_t n);
int cstr_pop(cstr_t *self);
int cstr_copy(cstr_t *self, const char *str, size_t n);
char cstr_at(cstr_t *self, size_t index);
int cstr_substr(cstr_t *self, size_t start, size_t end, cstr_t *out);
int cstr_reverse(cstr_t *self);
int cstr_remove(cstr_t *self, size_t start, size_t len);
int cstr_split(cstr_t *self, const char *delim, cstr_t **out, size_t *count);
int cstr_clear(cstr_t *self);
int cstr_free(cstr_t *self);
int cstr_shrink_to_fit(cstr_t *self);