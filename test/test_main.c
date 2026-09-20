#include "framework/framework.h"

void run_lexer_tokenize_tests(void);
void run_parser_ast_tests(void);

int main(void)
{
	TEST_BEGIN();

	run_lexer_tokenize_tests();
	run_parser_ast_tests();

	TEST_END();
	return g_tests_failed;
}
