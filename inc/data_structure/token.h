#pragma once

#include "types.h"
#include "data_structure/cstr.h"

typedef enum tok_type {
    TOK_STR,
    TOK_REDIR_FD,
    TOK_REDIR_OUT,
    TOK_REDIR_OUT_APPEND,
    TOK_PIPE,
    TOK_OR,
    TOK_AMPERSAND,
} tok_type_t;

typedef struct token {
    tok_type_t type;
    cstr_t val;
} token_t;

void token_deinit(token_t *self);
void token_clear(token_t *self);
int token_copy(token_t *dest, const token_t *src);
void token_init(token_t *self);
token_t* token_clone(const token_t *src);