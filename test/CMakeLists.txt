set(LEXER_TEST_SOURCES
    test_main.c
    framework/framework.c
    lexer/test_lexer_tokenize.c
    parser/test_parser_ast.c
    ../src/core/lexer.c
    ../src/core/parser.c
    ../src/helper/string_helper.c
    ../src/data_structure/cstr.c
    ../src/data_structure/vec.c
    ../src/data_structure/token.c
    ../src/data_structure/ast.c
)

add_executable(shell_unit_tests ${LEXER_TEST_SOURCES})
target_include_directories(shell_unit_tests PRIVATE ../inc .)

add_test(NAME shell_unit_tests COMMAND shell_unit_tests)
