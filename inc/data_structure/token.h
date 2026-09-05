#pragma once

#include "types.h"
#include "data_structure/cstr.h"

void token_free(token_t *self);
void token_clear(token_t *self);
int token_copy(token_t *dest, const token_t *src);
void token_init(token_t *self);