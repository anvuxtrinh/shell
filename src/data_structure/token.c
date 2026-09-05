#include <errno.h>
#include "data_structure/token.h"

void token_free(token_t *self) {
    if(self == NULL) {
        return;
    }

    cstr_clear(&self->val);
}

void token_clear(token_t *self) {
    if(self == NULL) {
        return;
    }

    self->type = 0;
    cstr_clear(&self->val);
}

int token_copy(token_t *dest, const token_t *src) {
    if(dest == NULL || src == NULL) {
        return EINVAL;
    }

    dest->type = src->type;
    return cstr_copy(&dest->val, src->val.data, src->val.len);
}

void token_init(token_t *self) {
    if(self == NULL) {
        return;
    }

    self->type = 0;
    self->val = (cstr_t){0};
}