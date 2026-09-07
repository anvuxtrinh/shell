#pragma once

#include <stdio.h>
#include <setjmp.h>
#include <string.h>

// ================ Testing state management ================
extern int g_tests_run;
extern int g_tests_failed;
extern jmp_buf g_test_env;

// ================ Setup and teardown function pointers ================
extern void (*g_setUp)(void);
extern void (*g_tearDown)(void);

// ================ Color codes for terminal output ================
#define COLOR_RESET   "\033[0m"
#define COLOR_BOLD    "\033[1m"
#define COLOR_RED     "\033[38;5;203m" // coral red for failures
#define COLOR_GREEN   "\033[38;5;84m"  // soft green for passes
#define COLOR_YELLOW  "\033[38;5;221m" // amber for warnings
#define COLOR_CYAN    "\033[38;5;117m" // Unity-inspired cyan for info
#define COLOR_MAGENTA "\033[38;5;201m" // magenta for special cases

// ================ Setup and teardown macros ================
#define SET_SETUP(func)    g_setUp = func
#define SET_TEARDOWN(func) g_tearDown = func

// ================ Test macros ================
#define TEST_BEGIN() printf(COLOR_BOLD COLOR_CYAN "================ RUNNING TESTS ================\n" COLOR_RESET)
#define TEST_END()   printf(COLOR_BOLD COLOR_CYAN "===============================================\n" COLOR_RESET \
                            "SUMMARY:" COLOR_RESET " %d " COLOR_BOLD COLOR_MAGENTA "Executed" COLOR_RESET " | %d " COLOR_BOLD COLOR_GREEN "Passed" COLOR_RESET " | %d " COLOR_BOLD COLOR_RED "Failed" COLOR_RESET "\n", \
                            g_tests_run, g_tests_run - g_tests_failed, g_tests_failed)

#define RUN_TEST(test_func) do { \
    g_tests_run++; \
    if(g_setUp) g_setUp(); \
    if(setjmp(g_test_env) == 0) { \
        test_func(); \
        printf(COLOR_BOLD COLOR_GREEN "[PASS] " COLOR_RESET "%s\n", #test_func); \
    } else { \
        g_tests_failed++; \
        printf(COLOR_BOLD COLOR_RED "[FAIL] " COLOR_RESET "%s\n", #test_func); \
    } \
    if(g_tearDown) g_tearDown(); \
} while(0)

#define ASSERT(expr, errmsg) do { \
    if(!(expr)) { \
        printf("Assertion failed: %s, function %s, file %s, line %d. Message: %s\n", \
               #expr, __func__, __FILE__, __LINE__, errmsg); \
        longjmp(g_test_env, 1); \
    } \
} while(0)

#define ASSERT_EQ(expected, actual, errmsg) do { \
    if((expected) != (actual)) { \
        printf("Assertion failed: expected %d, got %d, function %s, file %s, line %d. Message: %s\n", \
               (expected), (actual), __func__, __FILE__, __LINE__, errmsg); \
        longjmp(g_test_env, 1); \
    } \
} while(0)

#define ASSERT_PTR_EQ(expected, actual, errmsg) do { \
    if((expected) != (actual)) { \
        printf("Assertion failed: expected pointer %p, got %p, function %s, file %s, line %d. Message: %s\n", \
               (void *)(expected), (void *)(actual), __func__, __FILE__, __LINE__, errmsg); \
        longjmp(g_test_env, 1); \
    } \
} while(0)

#define ASSERT_NOT_NULL(ptr, errmsg) do { \
    if((ptr) == NULL) { \
        printf("Assertion failed: pointer is NULL, function %s, file %s, line %d. Message: %s\n", \
               __func__, __FILE__, __LINE__, errmsg); \
        longjmp(g_test_env, 1); \
    } \
} while(0)

#define ASSERT_NULL(ptr, errmsg) do { \
    if((ptr) != NULL) { \
        printf("Assertion failed: pointer is not NULL, function %s, file %s, line %d. Message: %s\n", \
               __func__, __FILE__, __LINE__, errmsg); \
        longjmp(g_test_env, 1); \
    } \
} while(0)

#define ASSERT_TRUE(expr, errmsg) do { \
    if(!(expr)) { \
        printf("Assertion failed: expected true, got false, function %s, file %s, line %d. Message: %s\n", \
               __func__, __FILE__, __LINE__, errmsg); \
        longjmp(g_test_env, 1); \
    } \
} while(0)

#define ASSERT_FALSE(expr, errmsg) do { \
    if((expr)) { \
        printf("Assertion failed: expected false, got true, function %s, file %s, line %d. Message: %s\n", \
               __func__, __FILE__, __LINE__, errmsg); \
        longjmp(g_test_env, 1); \
    } \
} while(0)

#define ASSERT_STR_EQ(expected, actual, errmsg) do { \
    if(strcmp((expected), (actual)) != 0) { \
        printf("Assertion failed: expected string \"%s\", got \"%s\", function %s, file %s, line %d. Message: %s\n", \
               (expected), (actual), __func__, __FILE__, __LINE__, errmsg); \
        longjmp(g_test_env, 1); \
    } \
} while(0)

#define ASSERT_STR_NOT_NULL(str, errmsg) do { \
    if((str) == NULL) { \
        printf("Assertion failed: string is NULL, function %s, file %s, line %d. Message: %s\n", \
               __func__, __FILE__, __LINE__, errmsg); \
        longjmp(g_test_env, 1); \
    } \
} while(0)

#define ASSERT_STR_NULL(str, errmsg) do { \
    if((str) != NULL) { \
        printf("Assertion failed: string is not NULL, function %s, file %s, line %d. Message: %s\n", \
               __func__, __FILE__, __LINE__, errmsg); \
        longjmp(g_test_env, 1); \
    } \
} while(0)

#define ASSERT_STRUCT_EQ(expected, actual, struct_type, errmsg) do { \
    if(memcmp(&(expected), &(actual), sizeof(struct_type)) != 0) { \
        printf("Assertion failed: expected struct does not match actual struct, function %s, file %s, line %d. Message: %s\n", \
               __func__, __FILE__, __LINE__, errmsg); \
        longjmp(g_test_env, 1); \
    } \
} while(0)