#pragma once

#include "types.h"
#include "data_structure/vec.h"
#include "core/token.h"
#include "core/ast.h"

ast_node_t *parser_parse(vec_t *tok_list);