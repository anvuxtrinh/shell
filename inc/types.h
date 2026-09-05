#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "data_structure/cstr.h"

// ========== Basic Type Definitions ==========
typedef int32_t i32;
typedef uint32_t u32;
typedef int8_t i8;
typedef uint8_t u8;
typedef int64_t i64;
typedef uint64_t u64;
typedef bool b8;

// ========== LOG Definitions ==========
#if LOG_LEVEL >= 1
#define LOG_INFO(...)  printf("[INFO] " __VA_ARGS__)
#else
#define LOG_INFO(...)
#endif

#if LOG_LEVEL >= 2
#define LOG_WARN(...)  printf("[WARN] " __VA_ARGS__)
#else
#define LOG_WARN(...)
#endif

#if LOG_LEVEL >= 3
#define LOG_ERROR(...) printf("[ERROR] " __VA_ARGS__)
#else
#define LOG_ERROR(...)
#endif

// ========== Token Type Definitions ==========
typedef enum tok_type {
    TOK_STR,
    TOK_REDIR_FD,
    TOK_REDIR_OUT,
    TOK_REDIR_OUT_APPEND,
    TOK_PIPE,
    TOK_OR,
    TOK_AMPERSAND,
} tok_type_t;

typedef struct token {
    tok_type_t type;
    cstr_t val;
} token_t;
