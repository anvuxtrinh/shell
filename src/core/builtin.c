#include "core/builtin.h"
#include "data_structure/hashmap.h"

typedef void (*builtin_handler_t)(const vec_t *args);

typedef struct {
    const char *name;
    builtin_handler_t handler;
} builtin_t;

static void builtin_echo(const vec_t *args);
static void builtin_type(const vec_t *args);
static void builtin_pwd(const vec_t *args);
static void builtin_cd(const vec_t *args);
static void builtin_exit(const vec_t *args);
static void builtin_history(const vec_t *args);
static void builtin_jobs(const vec_t *args);
static void builtin_complete(const vec_t *args);
static void builtin_declare(const vec_t *args);

static hashmap_t s_builtin_map;
static b8 s_builtin_map_initialized = false;

static builtin_t s_builtin_list[] = {
    {.name = "exit", .handler = builtin_exit},
    {.name = "echo", .handler = builtin_echo},
    {.name = "type", .handler = builtin_type},
    {.name = "pwd", .handler = builtin_pwd},
    {.name = "cd", .handler = builtin_cd},
    {.name = "history", .handler = builtin_history},
    {.name = "jobs", .handler = builtin_jobs},
    {.name = "complete", .handler = builtin_complete},
    {.name = "declare", .handler = builtin_declare},
};

i32 builtin_init() {
    if (s_builtin_map_initialized) {
        return 0; // Already initialized
    }

    hashmap_init(&s_builtin_map);

    for (size_t i = 0; i < sizeof(s_builtin_list) / sizeof(s_builtin_list[0]); ++i) {
        hashmap_put(&s_builtin_map, (const void *)s_builtin_list[i].name, strlen(s_builtin_list[i].name) + 1, (void *)s_builtin_list[i].handler);
    }

    s_builtin_map_initialized = true;
    return 0;
}

u32 builtin_exec(const vec_t *args, b8 async) {
    (void)async;
    if (args->size == 0) {
        return EINVAL;
    }

    if (!s_builtin_map_initialized) {
        builtin_init();
    }

    const cstr_t *key = (const cstr_t*)vec_at((const vec_t *)args, 0);
    builtin_handler_t handler = (builtin_handler_t)hashmap_get(&s_builtin_map, key->data, key->len+1);
    if(handler) {
        handler((const vec_t *)args);
        return 0;
    }

    return ENOENT;
}

static void builtin_echo(const vec_t *args) {
    printf("Echo command executed with %zu arguments.\n", args->size - 1);
}

static void builtin_type(const vec_t *args) {
    printf("Type command executed with %zu arguments.\n", args->size - 1);
}

static void builtin_pwd(const vec_t *args) {
    printf("PWD command executed with %zu arguments.\n", args->size - 1);
}

static void builtin_cd(const vec_t *args) {
    printf("CD command executed with %zu arguments.\n", args->size - 1);
}

static void builtin_history(const vec_t *args) {
    printf("History command executed with %zu arguments.\n", args->size - 1);
}

static void builtin_jobs(const vec_t *args) {
    printf("Jobs command executed with %zu arguments.\n", args->size - 1);
}

static void builtin_exit(const vec_t *args) {
    printf("Exit command executed with %zu arguments.\n", args->size - 1);
}

static void builtin_complete(const vec_t *args) {
    printf("Complete command executed with %zu arguments.\n", args->size - 1);
}

static void builtin_declare(const vec_t *args) {
    printf("Declare command executed with %zu arguments.\n", args->size - 1);
}


