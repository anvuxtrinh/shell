#include "framework/framework.h"

void run_lexer_tokenize_tests(void);

int main(void)
{
	TEST_BEGIN();

	run_lexer_tokenize_tests();

	TEST_END();
	return g_tests_failed;
}
