#pragma once

#include "types.h"
#include "data_structure/vec.h"
#include "data_structure/cstr.h"

typedef enum ast_node_type {
    NODE_TYPE_UNKNOWN = 0,
    NODE_TYPE_COMMAND,
    NODE_TYPE_CONNECTION,
    NODE_TYPE_BG,
    NODE_TYPE_PIPE,
    NODE_TYPE_AND,
    NODE_TYPE_COUNT
} ast_node_type_t;

typedef struct redirection {
    cstr_t value;
    cstr_t file;
    i32 fd;
} redirection_t;

typedef struct command{
    vec_t args;
    vec_t redirs;
} command_t;

typedef struct ast_node {
    ast_node_type_t type;
    union {
        command_t single_command;
        struct {
            struct ast_node *left;
            struct ast_node *right;
        } connection;
    };
} ast_node_t;

void ast_node_init(ast_node_t *self, ast_node_type_t type);
void ast_node_deinit(ast_node_t *self);
void print_ast(const ast_node_t *node, int depth);