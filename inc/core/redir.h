#pragma once

#include "types.h"
#include "data_structure/cstr.h"

#define REDIR_OUT 1
#define REDIR_OUT_APPEND 2

typedef struct redirection {
    i32 type; // REDIR_OUT or REDIR_OUT_APPEND
    cstr_t file;
    i32 fd;
} redirection_t;

i32 redirection_init(redirection_t *redir, i32 type, const cstr_t *file, i32 fd);
void redirection_deinit(redirection_t *redir);
redirection_t *redirection_clone(const redirection_t *redir);