#include "data_structure/type_ops.h"
#include "data_structure/ast.h"
#include "data_structure/cstr.h"

static void command_init(command_t *self) {
    if (self == NULL) {
        return;
    }
    vec_init(&self->args, sizeof(cstr_t), &cstr_ops);
}

void ast_node_init(ast_node_t *self, ast_node_type_t type) {
    if (self == NULL) {
        return;
    }

    memset(self, 0, sizeof(ast_node_t));
    self->type = type;
    if(type == NODE_TYPE_COMMAND) {
        command_init(&self->single_command);
    }
}

void ast_node_deinit(ast_node_t *self) {
    if (self == NULL) {
        return;
    }

    if (self->type == NODE_TYPE_COMMAND) {
        vec_deinit(&self->single_command.args);
        vec_deinit(&self->single_command.redirs);
    } else if (self->type == NODE_TYPE_CONNECTION) {
        ast_node_deinit(self->connection.left);
        ast_node_deinit(self->connection.right);
    }

    self->type = NODE_TYPE_UNKNOWN;
}

void print_ast(const ast_node_t *node, int depth) {
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < depth; ++i) {
        printf("  ");
    }

    switch (node->type) {
        case NODE_TYPE_COMMAND:
            printf("Command Node:\n");
            for (size_t i = 0; i < node->single_command.args.size; ++i) {
                cstr_t *arg = (cstr_t *)vec_at(&node->single_command.args, i);
                for (int j = 0; j < depth + 1; ++j) {
                    printf("  ");
                }
                printf("Arg %zu: %s\n", i, arg->data);
            }
            break;
        case NODE_TYPE_CONNECTION:
            printf("Connection Node:\n");
            print_ast(node->connection.left, depth + 1);
            print_ast(node->connection.right, depth + 1);
            break;
        default:
            printf("Unknown Node Type\n");
            break;
    }
}
