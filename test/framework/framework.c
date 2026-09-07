#include "framework.h"

int g_tests_run = 0;
int g_tests_failed = 0;
jmp_buf g_test_env;
void (*g_setUp)(void) = NULL;
void (*g_tearDown)(void) = NULL;