#include <errno.h>
#include "core/parser.h"

static i32 parse_command(vec_t *tok_list, ast_node_t *ast) {
    if (tok_list == NULL || ast == NULL) {
        return EINVAL;
    }

    ast_node_init(ast, NODE_TYPE_COMMAND);

    for(i32 i = 0; i < tok_list->size; i++) {
        token_t *tok = (token_t*)vec_at(tok_list, i);
        if(tok == NULL) {
            continue;
        }
        
        if(tok->type == TOK_STR) {
            ast->type = NODE_TYPE_COMMAND;
            vec_push(&ast->single_command.args, &tok->val);
        } else {
            break;
        }
    }

    return 0;
}

i32 parser_parse(vec_t *tok_list, ast_node_t *ast) {
    if (!tok_list || !ast) {
        return EINVAL;
    }

    return parse_command(tok_list, ast);
}