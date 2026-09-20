#pragma once

#include <stdint.h>
#include <stdbool.h>

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



