#include <errno.h>
#include <string.h>

#include "../framework/framework.h"
#include "../../inc/core/lexer.h"

// = Test Fixtures
static cstr_t g_input;
static vec_t g_tok_list;

static void setup(void)
{
    g_input = (cstr_t){0};
    g_tok_list = (vec_t){0};

    int ret = vec_init(&g_tok_list, sizeof(token_t), (vec_free_cb_t)token_free);
    ASSERT_EQ(0, ret, "Expected vec_init to succeed for token list fixture");
}

static void teardown(void)
{
    int ret = vec_free(&g_tok_list);
    ASSERT_EQ(0, ret, "Expected vec_free to succeed during fixture teardown");

    ret = cstr_free(&g_input);
    ASSERT_EQ(0, ret, "Expected cstr_free to succeed during fixture teardown");
}

static void set_input(const char *text)
{
    int ret = cstr_copy(&g_input, text, strlen(text));
    ASSERT_EQ(0, ret, "Expected cstr_copy to succeed when preparing input");
}

static void assert_token(size_t index, const char *expected)
{
    token_t *tok = (token_t *)vec_at(&g_tok_list, index);
    ASSERT_NOT_NULL(tok, "Expected token pointer from vec_at to be non-NULL");
    ASSERT_EQ(TOK_STR, tok->type, "Expected token type to be TOK_STR");
    ASSERT_STR_EQ(expected, tok->val.data, "Expected token value to match input segment");
}

// = Null Input Tests
/**
 * @brief Verify lex_tokenize rejects a NULL input pointer.
 * Preconditions: token list fixture is initialized.
 * Action: call lex_tokenize with input = NULL.
 * Expected: returns -EINVAL and token list remains unchanged.
 */
void test_lexer_tokenize_null_input_returns_neg_einval(void)
{
    int ret = lex_tokenize(NULL, &g_tok_list);
    ASSERT_EQ(EINVAL, ret, "Expected -EINVAL when input pointer is NULL");
    ASSERT_EQ(0, (int)g_tok_list.size, "Expected no tokens to be added on NULL input");
}

/**
 * @brief Verify lex_tokenize rejects a NULL token list pointer.
 * Preconditions: input fixture is initialized with valid text.
 * Action: call lex_tokenize with tok_list = NULL.
 * Expected: returns -EINVAL.
 */
void test_lexer_tokenize_null_tok_list_returns_neg_einval(void)
{
    set_input("echo");

    int ret = lex_tokenize(&g_input, NULL);
    ASSERT_EQ(EINVAL, ret, "Expected -EINVAL when token list pointer is NULL");
}

// = Boundary Condition Tests
/**
 * @brief Verify lex_tokenize handles empty input without generating tokens.
 * Preconditions: input fixture contains empty string.
 * Action: tokenize empty input.
 * Expected: returns success and token list size stays zero.
 */
void test_lexer_tokenize_empty_input_produces_no_tokens(void)
{
    set_input("");

    int ret = lex_tokenize(&g_input, &g_tok_list);
    ASSERT_EQ(0, ret, "Expected success when tokenizing empty input");
    ASSERT_EQ(0, (int)g_tok_list.size, "Expected no tokens for empty input");
}

/**
 * @brief Verify lex_tokenize ignores leading whitespace-only input.
 * Preconditions: input fixture contains spaces, tabs, and newlines only.
 * Action: tokenize whitespace-only input.
 * Expected: returns success and no tokens are produced.
 */
void test_lexer_tokenize_whitespace_only_input_produces_no_tokens(void)
{
    set_input("  \t\n\t  \n");

    int ret = lex_tokenize(&g_input, &g_tok_list);
    ASSERT_EQ(0, ret, "Expected success when tokenizing whitespace-only input");
    ASSERT_EQ(0, (int)g_tok_list.size, "Expected no tokens for whitespace-only input");
}

/**
 * @brief Verify lex_tokenize does not create empty tokens for repeated separators.
 * Preconditions: input contains multiple sequential separators between words.
 * Action: tokenize "a   b\t\tc".
 * Expected: exactly three non-empty tokens are produced.
 */
void test_lexer_tokenize_consecutive_separators_do_not_create_empty_tokens(void)
{
    set_input("a   b\t\tc");

    int ret = lex_tokenize(&g_input, &g_tok_list);
    ASSERT_EQ(0, ret, "Expected success for repeated separators in input");
    ASSERT_EQ(3, (int)g_tok_list.size, "Expected exactly three tokens for input 'a   b\\t\\tc'");
    assert_token(0, "a");
    assert_token(1, "b");
    assert_token(2, "c");
}

// = Valid Behavior Tests (Happy Path)
/**
 * @brief Verify lex_tokenize produces one token for a single-word command.
 * Preconditions: input fixture contains "echo".
 * Action: tokenize the input.
 * Expected: success return code and one token with value "echo".
 */
void test_lexer_tokenize_single_word(void)
{
    set_input("echo");

    int ret = lex_tokenize(&g_input, &g_tok_list);
    ASSERT_EQ(0, ret, "Expected success for single-word input");
    ASSERT_EQ(1, (int)g_tok_list.size, "Expected one token for single-word input");
    assert_token(0, "echo");
}

/**
 * @brief Verify lex_tokenize splits words with mixed whitespace delimiters.
 * Preconditions: input fixture contains leading/trailing spaces and mixed delimiters.
 * Action: tokenize "  ls\t-la\n/tmp  ".
 * Expected: success and exactly three tokens preserving each segment.
 */
void test_lexer_tokenize_multiple_words_mixed_whitespace(void)
{
    set_input("  ls\t-la\n/tmp  ");

    int ret = lex_tokenize(&g_input, &g_tok_list);
    ASSERT_EQ(0, ret, "Expected success for mixed-whitespace command input");
    ASSERT_EQ(3, (int)g_tok_list.size, "Expected three tokens for mixed-whitespace input");
    assert_token(0, "ls");
    assert_token(1, "-la");
    assert_token(2, "/tmp");
}

// = State Transition Tests
/**
 * @brief Verify tokenization works correctly after clearing and reusing token list.
 * Preconditions: first tokenization succeeds and token list is then cleared.
 * Action: tokenize a second valid input using the same fixtures.
 * Expected: second run succeeds and token list reflects only second input.
 */
void test_lexer_tokenize_clear_then_reuse_tok_list(void)
{
    set_input("first pass");
    int ret = lex_tokenize(&g_input, &g_tok_list);
    ASSERT_EQ(0, ret, "Expected first tokenization to succeed before clear");
    ASSERT_EQ(2, (int)g_tok_list.size, "Expected two tokens after first tokenization");

    ret = vec_clear(&g_tok_list);
    ASSERT_EQ(0, ret, "Expected vec_clear to succeed before second tokenization");

    ret = cstr_copy(&g_input, "second", strlen("second"));
    ASSERT_EQ(0, ret, "Expected cstr_copy to succeed for second input");

    ret = lex_tokenize(&g_input, &g_tok_list);
    ASSERT_EQ(0, ret, "Expected second tokenization to succeed after clear");
    ASSERT_EQ(1, (int)g_tok_list.size, "Expected one token after second tokenization");
    assert_token(0, "second");
}

// = Error Code Verification Tests
/**
 * @brief Verify invalid argument paths return POSIX -EINVAL.
 * Preconditions: input and token list fixtures are initialized.
 * Action: call lex_tokenize with each invalid pointer permutation.
 * Expected: each invalid-argument call returns -EINVAL.
 */
void test_lexer_tokenize_invalid_argument_error_codes(void)
{
    int ret = lex_tokenize(NULL, &g_tok_list);
    ASSERT_EQ(EINVAL, ret, "Expected -EINVAL for NULL input pointer");

    set_input("pwd");
    ret = lex_tokenize(&g_input, NULL);
    ASSERT_EQ(EINVAL, ret, "Expected -EINVAL for NULL token list pointer");
}

void run_lexer_tokenize_tests(void)
{
    SET_SETUP(setup);
    SET_TEARDOWN(teardown);

    RUN_TEST(test_lexer_tokenize_null_input_returns_neg_einval);
    RUN_TEST(test_lexer_tokenize_null_tok_list_returns_neg_einval);
    RUN_TEST(test_lexer_tokenize_empty_input_produces_no_tokens);
    RUN_TEST(test_lexer_tokenize_whitespace_only_input_produces_no_tokens);
    RUN_TEST(test_lexer_tokenize_consecutive_separators_do_not_create_empty_tokens);
    RUN_TEST(test_lexer_tokenize_single_word);
    RUN_TEST(test_lexer_tokenize_multiple_words_mixed_whitespace);
    RUN_TEST(test_lexer_tokenize_clear_then_reuse_tok_list);
    RUN_TEST(test_lexer_tokenize_invalid_argument_error_codes);
}
