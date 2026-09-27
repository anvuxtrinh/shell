#pragma once

#include "core/token.h"
#include "core/redir.h"
#include "data_structure/cstr.h"

typedef void  (*data_init_cb_t)(void *elem);
typedef void* (*data_clone_cb_t)(const void *elem);
typedef void  (*data_deinit_cb_t)(void *elem);

typedef struct data_ops {
    data_clone_cb_t clone;
    data_init_cb_t init;
    data_deinit_cb_t deinit;
} data_ops_t;

static const data_ops_t token_ops = {
    .clone = (data_clone_cb_t)token_clone,
    .init = (data_init_cb_t)token_init,
    .deinit = (data_deinit_cb_t)token_deinit
};

static const data_ops_t cstr_ops = {
    .clone = (data_clone_cb_t)cstr_clone,
    .init = (data_init_cb_t)cstr_init,
    .deinit = (data_deinit_cb_t)cstr_deinit
};

static const data_ops_t redir_ops = {
    .clone = (data_clone_cb_t)redirection_clone,
    .init = NULL,
    .deinit = (data_deinit_cb_t)redirection_deinit
};