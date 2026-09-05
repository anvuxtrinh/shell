#include "helper/string_helper.h"

size_t find_first_non_whitespace(const char *str, size_t len) {
    if(str == NULL || len == 0) {
        return 0;
    }

    for(size_t i = 0; i < len; ++i) {
        if(str[i] != ' ' && str[i] != '\t' && str[i] != '\n') {
            return i;
        }
    }

    return len;
}