#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "data_structure/cstr.h"

#define CSTR_INITIAL_CAPACITY 4
#define CSTR_GROWTH_FACTOR 2
#define CSTR_SHRINK_THRESHOLD 2

static inline bool should_grow(cstr_t *self, size_t additional_len) {
    return self->cap == 0 || (self->len + additional_len + 1) > self->cap;
}

static inline bool should_shrink(cstr_t *self) {
    return self->cap > CSTR_INITIAL_CAPACITY && self->len < self->cap / CSTR_SHRINK_THRESHOLD;
}

static int cstr_grow_capacity(cstr_t *self, size_t additional_len) {
    size_t new_cap = self->cap == 0 ? CSTR_INITIAL_CAPACITY : self->cap;
    while ((self->len + additional_len + 1) > new_cap) {
        new_cap *= CSTR_GROWTH_FACTOR;
    }

    char *new_data = (char *)realloc(self->data, new_cap);
    if (new_data == NULL) {
        return ENOMEM;
    }

    self->data = new_data;
    self->cap = new_cap;
    return 0;
}

static int cstr_shrink_capacity(cstr_t *self) {
    size_t new_cap = self->cap / CSTR_GROWTH_FACTOR;
    new_cap = new_cap < CSTR_INITIAL_CAPACITY ? CSTR_INITIAL_CAPACITY : new_cap;

    char *new_data = (char *)realloc(self->data, new_cap);
    if (new_data == NULL) {
        return ENOMEM;
    }

    self->data = new_data;
    self->cap = new_cap;
    return 0;
}

int cstr_appendn(cstr_t *self, const char *str, size_t n) {
    if(self == NULL || str == NULL ||
        n == 0 || n > SIZE_MAX - self->len - 1) return EINVAL;

    if(should_grow(self, n)) {
        int ret = cstr_grow_capacity(self, n);
        if(ret != 0) { return ret; }
    }

    memcpy(self->data + self->len, str, n);
    self->len += n;
    self->data[self->len] = '\0';
    return 0;
}

int cstr_pop(cstr_t *self) {
    if(self == NULL || self->data == NULL || 
        self->len == 0) return EINVAL;

    self->len--;
    self->data[self->len] = '\0';

    if(should_shrink(self)) {
        (void) cstr_shrink_capacity(self);
    }

    return 0;
}

int cstr_copy(cstr_t *self, const char *str, size_t n) {
    if(self == NULL || str == NULL || 
        n > SIZE_MAX - 1) return EINVAL;

    if(n == 0) {
        self->len = 0;
        if(self->data != NULL) {
            self->data[0] = '\0';
        }
        return 0;
    }

    if(should_grow(self, n)) {
        int ret = cstr_grow_capacity(self, n);
        if(ret != 0) { return ret; }
    }

    memcpy(self->data, str, n);
    self->len = n;
    self->data[self->len] = '\0';
    return 0;
}

char cstr_at(cstr_t *self, size_t index) {
    if(self == NULL || index >= self->len) { return '\0'; }
    return self->data[index];
}

int cstr_substr(cstr_t *self, size_t start, size_t end, cstr_t *out) {
    if(self == NULL || self->data == NULL || out == NULL ||
       start >= self->len || end > self->len || start >= end) { 
        return EINVAL; 
    }

    size_t substr_len = end - start;
    int ret = cstr_copy(out, self->data + start, substr_len);
    if(ret != 0) { return ret; }

    return 0;
}

int cstr_reverse(cstr_t *self) {
    if(self == NULL || self->data == NULL) { return EINVAL; }

    for(size_t i = 0; i < self->len / 2; i++) {
        char temp = self->data[i];
        self->data[i] = self->data[self->len - 1 - i];
        self->data[self->len - 1 - i] = temp;
    }

    return 0;
}

int cstr_remove(cstr_t *self, size_t start, size_t len) {
    if(self == NULL || self->data == NULL) { return EINVAL; }
    if(start >= self->len || start + len > self->len) { return EINVAL; }

    memmove(self->data + start, self->data + start + len, self->len - (start + len));
    self->len -= len;
    self->data[self->len] = '\0';

    if(should_shrink(self)) {
        (void) cstr_shrink_capacity(self);
    }

    return 0;
}

int cstr_clear(cstr_t *self) {
    if(self == NULL) { return EINVAL; }
    if(self->data != NULL) {
        self->data[0] = '\0';
    }
    self->len = 0;
    return 0;
}

int cstr_free(cstr_t *self) {
    if(self == NULL) { return EINVAL; }
    if(self->data != NULL) {
        free(self->data);
        self->data = NULL;
    }
    self->len = 0;
    self->cap = 0;
    return 0;
}

int cstr_shrink_to_fit(cstr_t *self) {
    if(self == NULL) { return EINVAL; }

    if(self->len == 0) {
        if(self->data != NULL) {
            free(self->data);
            self->data = NULL;
        }
        self->cap = 0;
        return 0;
    }

    size_t new_cap = self->len + 1;
    char *new_data = (char *)realloc(self->data, new_cap);
    if (new_data == NULL) { return ENOMEM; }

    self->data = new_data;
    self->cap = new_cap;
    return 0;
}