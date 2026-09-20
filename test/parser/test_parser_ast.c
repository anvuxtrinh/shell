#include <errno.h>
#include <string.h>

#include "../framework/framework.h"
#include "../../inc/core/parser.h"
#include "../../inc/data_structure/type_ops.h"

// = Test Fixtures
static vec_t g_tok_list;
static ast_node_t g_ast;

static void setup(void)
{
    g_tok_list = (vec_t){0};
    g_ast = (ast_node_t){0};

    int ret = vec_init(&g_tok_list, sizeof(token_t), &token_ops);
    ASSERT_EQ(0, ret, "Expected vec_init to succeed for token list fixture");
}

static void teardown(void)
{
    int ret = vec_deinit(&g_tok_list);
    ASSERT_EQ(0, ret, "Expected vec_deinit to succeed during fixture teardown");

    ast_node_deinit(&g_ast);
}

static void add_token(tok_type_t type, const char *val)
{
    token_t tok;
    token_init(&tok);
    tok.type = type;

    if (val != NULL) {
        int ret = cstr_copy(&tok.val, val, strlen(val));
        ASSERT_EQ(0, ret, "Expected cstr_copy to succeed when preparing token value");
    }

    int ret = vec_push(&g_tok_list, &tok);
    ASSERT_EQ(0, ret, "Expected vec_push to succeed adding token to list");

    token_deinit(&tok);
}

static void assert_arg(size_t index, const char *expected)
{
    const cstr_t *arg = (const cstr_t *)vec_at(&g_ast.single_command.args, index);
    ASSERT_NOT_NULL(arg, "Expected arg pointer from vec_at to be non-NULL");
    ASSERT_STR_EQ(expected, arg->data, "Expected arg value to match expected token value");
}

// = Null Input Tests
/**
 * @brief Verify parser_parse rejects a NULL token list pointer.
 * Preconditions: ast fixture is zero-initialized.
 * Action: call parser_parse with tok_list = NULL.
 * Expected: returns EINVAL.
 */
void test_parser_parse_null_tok_list_returns_einval(void)
{
    int ret = parser_parse(NULL, &g_ast);
    ASSERT_EQ(EINVAL, ret, "Expected EINVAL when token list pointer is NULL");
}

/**
 * @brief Verify parser_parse rejects a NULL ast pointer.
 * Preconditions: token list fixture is initialized and empty.
 * Action: call parser_parse with ast = NULL.
 * Expected: returns EINVAL.
 */
void test_parser_parse_null_ast_returns_einval(void)
{
    int ret = parser_parse(&g_tok_list, NULL);
    ASSERT_EQ(EINVAL, ret, "Expected EINVAL when ast pointer is NULL");
}

/**
 * @brief Verify parser_parse rejects both pointers being NULL.
 * Preconditions: none.
 * Action: call parser_parse with tok_list = NULL and ast = NULL.
 * Expected: returns EINVAL.
 */
void test_parser_parse_both_null_returns_einval(void)
{
    int ret = parser_parse(NULL, NULL);
    ASSERT_EQ(EINVAL, ret, "Expected EINVAL when both pointers are NULL");
}

// = Boundary Condition Tests
/**
 * @brief Verify parser_parse handles an empty token list.
 * Preconditions: token list fixture is initialized and empty.
 * Action: call parser_parse with an empty token list.
 * Expected: returns success, ast becomes a command node with zero args.
 */
void test_parser_parse_empty_tok_list_produces_empty_command(void)
{
    int ret = parser_parse(&g_tok_list, &g_ast);
    ASSERT_EQ(0, ret, "Expected success when parsing an empty token list");
    ASSERT_EQ(NODE_TYPE_COMMAND, g_ast.type, "Expected ast node type to be NODE_TYPE_COMMAND");
    ASSERT_EQ(0, (int)g_ast.single_command.args.size, "Expected zero args for empty token list");
}

// = Valid Behavior Tests (Happy Path)
/**
 * @brief Verify parser_parse builds a command node from a single TOK_STR token.
 * Preconditions: token list contains one TOK_STR token "echo".
 * Action: call parser_parse.
 * Expected: returns success, ast is a command node with one arg "echo".
 */
void test_parser_parse_single_arg_command(void)
{
    add_token(TOK_STR, "echo");

    int ret = parser_parse(&g_tok_list, &g_ast);
    ASSERT_EQ(0, ret, "Expected success when parsing a single-token command");
    ASSERT_EQ(NODE_TYPE_COMMAND, g_ast.type, "Expected ast node type to be NODE_TYPE_COMMAND");
    ASSERT_EQ(1, (int)g_ast.single_command.args.size, "Expected one arg for single-token command");
    assert_arg(0, "echo");
}

/**
 * @brief Verify parser_parse builds a command node preserving arg order.
 * Preconditions: token list contains multiple TOK_STR tokens.
 * Action: call parser_parse with tokens "ls", "-la", "/tmp".
 * Expected: returns success, ast args match tokens in the same order.
 */
void test_parser_parse_multiple_arg_command(void)
{
    add_token(TOK_STR, "ls");
    add_token(TOK_STR, "-la");
    add_token(TOK_STR, "/tmp");

    int ret = parser_parse(&g_tok_list, &g_ast);
    ASSERT_EQ(0, ret, "Expected success when parsing a multi-token command");
    ASSERT_EQ(NODE_TYPE_COMMAND, g_ast.type, "Expected ast node type to be NODE_TYPE_COMMAND");
    ASSERT_EQ(3, (int)g_ast.single_command.args.size, "Expected three args for multi-token command");
    assert_arg(0, "ls");
    assert_arg(1, "-la");
    assert_arg(2, "/tmp");
}

// = State Transition / Non-Str Token Tests
/**
 * @brief Verify parser_parse stops collecting args at the first non-TOK_STR token.
 * Preconditions: token list contains TOK_STR tokens followed by a TOK_PIPE token
 * and a further TOK_STR token.
 * Action: call parser_parse.
 * Expected: returns success, only the leading TOK_STR tokens become args.
 */
void test_parser_parse_stops_at_non_str_token(void)
{
    add_token(TOK_STR, "cat");
    add_token(TOK_STR, "file.txt");
    add_token(TOK_PIPE, "|");
    add_token(TOK_STR, "grep");

    int ret = parser_parse(&g_tok_list, &g_ast);
    ASSERT_EQ(0, ret, "Expected success when parsing a command followed by a pipe token");
    ASSERT_EQ(NODE_TYPE_COMMAND, g_ast.type, "Expected ast node type to be NODE_TYPE_COMMAND");
    ASSERT_EQ(2, (int)g_ast.single_command.args.size, "Expected only leading TOK_STR tokens to become args");
    assert_arg(0, "cat");
    assert_arg(1, "file.txt");
}

/**
 * @brief Verify parser_parse produces zero args when the first token is not TOK_STR.
 * Preconditions: token list contains a single TOK_PIPE token.
 * Action: call parser_parse.
 * Expected: returns success, ast is a command node with zero args.
 */
void test_parser_parse_leading_non_str_token_produces_no_args(void)
{
    add_token(TOK_PIPE, "|");

    int ret = parser_parse(&g_tok_list, &g_ast);
    ASSERT_EQ(0, ret, "Expected success even when the first token is not TOK_STR");
    ASSERT_EQ(NODE_TYPE_COMMAND, g_ast.type, "Expected ast node type to be NODE_TYPE_COMMAND");
    ASSERT_EQ(0, (int)g_ast.single_command.args.size, "Expected zero args when leading token is not TOK_STR");
}

// = Ownership / Memory Independence Tests
/**
 * @brief Verify ast args are deep copies independent of the source token list.
 * Preconditions: token list contains one TOK_STR token "keep-me".
 * Action: parse, then deinit the source token list, then inspect ast args.
 * Expected: ast arg value remains valid and unchanged after token list teardown.
 */
void test_parser_parse_args_are_deep_copied_from_tokens(void)
{
    add_token(TOK_STR, "keep-me");

    int ret = parser_parse(&g_tok_list, &g_ast);
    ASSERT_EQ(0, ret, "Expected success when parsing single-token command");

    ret = vec_deinit(&g_tok_list);
    ASSERT_EQ(0, ret, "Expected vec_deinit to succeed on source token list");

    assert_arg(0, "keep-me");
}

void run_parser_ast_tests(void)
{
    SET_SETUP(setup);
    SET_TEARDOWN(teardown);

    RUN_TEST(test_parser_parse_null_tok_list_returns_einval);
    RUN_TEST(test_parser_parse_null_ast_returns_einval);
    RUN_TEST(test_parser_parse_both_null_returns_einval);

    RUN_TEST(test_parser_parse_empty_tok_list_produces_empty_command);

    RUN_TEST(test_parser_parse_single_arg_command);
    RUN_TEST(test_parser_parse_multiple_arg_command);

    RUN_TEST(test_parser_parse_stops_at_non_str_token);
    RUN_TEST(test_parser_parse_leading_non_str_token_produces_no_args);

    RUN_TEST(test_parser_parse_args_are_deep_copied_from_tokens);
}
