#include <errno.h>
#include "core/token.h"

void token_deinit(token_t *self) {
    if(self == NULL) {
        return;
    }

    self->type = 0;
    cstr_deinit(&self->val);
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
    if(dest->type == TOK_STR || dest->type == TOK_DIGIT) {
        return cstr_copy(&dest->val, src->val.data, src->val.len);
    }
    return 0;
}

void token_init(token_t *self) {
    if(self == NULL) {
        return;
    }

    self->type = 0;
    self->val = (cstr_t){0};
}

token_t* token_clone(const token_t *src) {
    if(src == NULL) {
        return NULL;
    }

    token_t *new_token = (token_t *)malloc(sizeof(token_t));
    if(new_token == NULL) {
        return NULL;
    }

    token_init(new_token);
    if(token_copy(new_token, src) != 0) {
        free(new_token);
        return NULL;
    }

    return new_token;
}