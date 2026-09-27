#include "core/redir.h"

i32 redirection_init(redirection_t *redir, i32 type, const cstr_t *file, i32 fd) {
    if(redir == NULL || file == NULL) {
        return -1;
    }

    redir->type = type;
    cstr_init(&redir->file);
    if(cstr_copy(&redir->file, file->data, file->len) != 0) {
        return -1;
    }

    redir->fd = fd;
    return 0;
}

void redirection_deinit(redirection_t *redir) {
    if (redir == NULL) {
        return;
    }
    cstr_deinit(&redir->file);
}

redirection_t *redirection_clone(const redirection_t *redir) {
    if (redir == NULL) {
        return NULL;
    }
    redirection_t *new_redir = (redirection_t *)malloc(sizeof(redirection_t));
    if (new_redir == NULL) {
        return NULL;
    }
    if (redirection_init(new_redir, redir->type, &redir->file, redir->fd) != 0) {
        free(new_redir);
        return NULL;
    }
    return new_redir;
}