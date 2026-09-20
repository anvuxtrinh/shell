#pragma once

#include "types.h"
#include "data_structure/vec.h"
#include "data_structure/token.h" 
#include "data_structure/ast.h"

i32 parser_parse(vec_t *tok_list, ast_node_t *ast);