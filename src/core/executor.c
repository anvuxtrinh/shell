#include <errno.h>
#include "core/executor.h"
#include "core/builtin.h"

i32 executor_exec(ast_node_t *ast) {
    if (ast == NULL) {
        return EINVAL;
    }

    // Check if the AST node is a command node
    if (ast->type != NODE_TYPE_COMMAND) {
        return EINVAL;
    }

    // Execute the command represented by the AST node
    u32 result = builtin_exec(&ast->single_command.args, false);
    return result;
}