#include <errno.h>
#include "core/parser.h"
#include "core/redir.h"

static ast_node_t *parse_command(vec_t *tok_list) {
    if (tok_list == NULL) {
        return NULL;
    }

    ast_node_t *ast = ast_node_create(NODE_TYPE_COMMAND);

    for (i32 i = 0; i < tok_list->size; i++) {
        token_t *tok = (token_t*)vec_at(tok_list, i);
        if(tok == NULL) {
            continue;
        }
        
        if(tok->type == TOK_STR) {
            ast->type = NODE_TYPE_COMMAND;
            vec_push(&ast->single_command.args, &tok->val);
            continue;
        }

        if(tok->type == TOK_DIGIT) {
            token_t *next_tok = (token_t*)vec_at(tok_list, i + 1);
            if(next_tok != NULL && (next_tok->type == TOK_REDIR_OUT || next_tok->type == TOK_REDIR_OUT_APPEND)) {
                token_t *next_next_tok = (token_t*)vec_at(tok_list, i + 2);
                if(next_next_tok != NULL && next_next_tok->type == TOK_STR) {
                    // Create a redirection structure and add it to the command's redirections
                    redirection_t redir;
                    redirection_init(&redir, cstr_to_num(&tok->val), &next_next_tok->val, (next_tok->type == TOK_REDIR_OUT) ? REDIR_OUT : REDIR_OUT_APPEND); // Assuming fd is 1 for stdout
                    vec_push(&ast->single_command.redirs, &redir);
                    i += 2; // Skip the next two tokens as they are part of the redirection
                } else {
                    // Handle error: expected a string token after redirection operator
                    return NULL;
                }
                continue;
            }else {
                ast->type = NODE_TYPE_COMMAND;
                vec_push(&ast->single_command.args, &tok->val);
                continue;
            }
        }
    }

    return ast;
}

ast_node_t *parser_parse(vec_t *tok_list) {
    if (!tok_list) {
        return NULL;
    }

    return parse_command(tok_list);
}