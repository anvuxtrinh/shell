#pragma once

#include "types.h"
#include "data_structure/cstr.h"
#include "data_structure/vec.h"
#include "core/token.h"

i32 lex_tokenize(cstr_t *input, vec_t *tok_list);