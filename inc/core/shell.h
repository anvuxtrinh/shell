#pragma once

#include "types.h"
#include "data_structure/cstr.h"
#include "data_structure/vec.h"

enum shell_state {
    SHELL_STATE_INIT = 0,
    SHELL_STATE_INTERACTIVE,
    SHELL_STATE_LEXING,
    SHELL_STATE_PARSING,
    SHELL_STATE_EXECUTING,
    SHELL_STATE_CLEANUP,
    SHELL_STATE_ERROR,
    SHELL_STATE_EXIT,
    SHELL_STATE_COUNT
};

struct shell_ctx{
    enum shell_state current_state;
    enum shell_state prev_state;

    // Data
    vec_t tok_list;
    cstr_t input_buf;

    //State
    b8 is_running;
    i32 exit_code;
};

void shell_context_init(struct shell_ctx *self);
void shell_context_free(struct shell_ctx *self);

i32 shell_run(struct shell_ctx *ctx);