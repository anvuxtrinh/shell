#pragma once

#include "data_structure/token.h"
#include "data_structure/cstr.h"

typedef void* (*data_clone_cb_t)(const void *elem);
typedef void  (*data_deinit_cb_t)(void *elem);

typedef struct data_ops {
    data_clone_cb_t clone;
    data_deinit_cb_t deinit;
} data_ops_t;

static const data_ops_t token_ops = {
    .clone = (data_clone_cb_t)token_clone,
    .deinit = (data_deinit_cb_t)token_deinit
};

static const data_ops_t cstr_ops = {
    .clone = (data_clone_cb_t)cstr_clone,
    .deinit = (data_deinit_cb_t)cstr_deinit
};