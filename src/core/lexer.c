#include <errno.h>
#include "core/lexer.h"
#include "helper/string_helper.h"

typedef enum lex_state {
    LEX_STATE_NORMAL,
    LEX_STATE_IN_SQUOTE,
    LEX_STATE_IN_DQUOTE,
    LEX_STATE_BACKSLASH,
    LEX_STATE_DOLLAR,
    LEX_STATE_DIGIT,
    LEX_STATE_COUNT
} lex_state_t;

typedef struct lex_ctx {
    cstr_t *input;
    vec_t *tok_list;
    token_t tok;
    size_t pos;
    lex_state_t state;
    lex_state_t prev_state;
    i32 exit_code;
} lex_ctx_t;

typedef void (*lex_state_func_t)(lex_ctx_t *ctx);

static void lex_state_normal(lex_ctx_t *ctx);

static const lex_state_func_t state_tbl[LEX_STATE_COUNT] = {
    [LEX_STATE_NORMAL] = lex_state_normal,
};

static inline b8 lex_end(lex_ctx_t *ctx){
    return ctx->pos >= ctx->input->len;
}

static inline char lex_next(lex_ctx_t *ctx) {
    if(lex_end(ctx)) {
        return '\0';
    }
    return ctx->input->data[ctx->pos++];
}

static inline char lex_peek(lex_ctx_t *ctx) {
    if(lex_end(ctx)) {
        return '\0';
    }
    return ctx->input->data[ctx->pos];
}

static inline void lex_back(lex_ctx_t *ctx) {
    if(ctx->pos > 0) {
        ctx->pos--;
    }
}

static inline void lex_state_transition(lex_ctx_t *ctx, lex_state_t new_state) {
    ctx->prev_state = ctx->state;
    ctx->state = new_state;
}

static inline void lex_ctx_init(lex_ctx_t *ctx, cstr_t *input, vec_t *tok_list) {
    ctx->input = input;
    ctx->tok_list = tok_list;
    ctx->tok = (token_t){0};
    ctx->pos = 0;
    ctx->state = LEX_STATE_NORMAL;
    ctx->prev_state = LEX_STATE_NORMAL;
    ctx->exit_code = 0;
}

static inline void lex_ctx_free(lex_ctx_t *ctx) {
    token_free(&ctx->tok);
}

static void lex_make_token(lex_ctx_t *ctx, tok_type_t type) {
    if(ctx == NULL || ctx->tok_list == NULL || ctx->input == NULL) {
        return;
    }

    if(type == TOK_REDIR_OUT || type == TOK_REDIR_OUT_APPEND 
        || type == TOK_REDIR_FD || type == TOK_PIPE || type == TOK_AMPERSAND
        || ctx->tok.val.len > 0) 
    {
        token_t buf_tok;
        token_init(&buf_tok);
        token_copy(&buf_tok, &ctx->tok);
        vec_push(ctx->tok_list, &buf_tok);
        token_clear(&ctx->tok);
    }
}

static void lex_state_normal(lex_ctx_t *ctx) {
    char c = lex_next(ctx);
    switch(c) {
        case ' ':
        case '\t':
        case '\n':
        case '\0':
            lex_make_token(ctx, TOK_STR);
            break;
        default:
            cstr_appendn(&ctx->tok.val, &c, 1);
            break;
    }
}

i32 lex_tokenize(cstr_t *input, vec_t *tok_list) {
    if(input == NULL || tok_list == NULL) {
        return EINVAL;
    }

    size_t st = find_first_non_whitespace(input->data, input->len);
    lex_ctx_t ctx = {
        .input = input,
        .tok_list = tok_list,
        .pos = st,
        .state = LEX_STATE_NORMAL,
        .prev_state = LEX_STATE_NORMAL,
        .exit_code = 0
    };

    while(!lex_end(&ctx)) {
        lex_state_func_t func = state_tbl[ctx.state];
        if(func == NULL) {
            return EINVAL;
        }
        func(&ctx);
    }

    if(ctx.tok.val.len > 0) {
        lex_make_token(&ctx, TOK_STR);
    }
    lex_ctx_free(&ctx);

    return ctx.exit_code;
}


