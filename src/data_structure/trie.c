#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "data_structure/trie.h"

#if defined UNIT_TEST
#define STATIC static
#define STATIC_INLINE static inline
#else
#define STATIC
#define STATIC_INLINE
#endif

#define CHAR_TO_INDEX(c) ((c) - 'a')
#define IS_VALID_INDEX(idx) ((idx) >= 0 && (idx) < ALPHABET_SIZE)

STATIC_INLINE void node_init_helper(trie_node_t *node) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        node->child[i] = NULL;
    }
    node->is_end_of_word = false;
}

int trie_init(trie_node_t *root) {
    if (root == NULL) {
        return EINVAL;
    }
    node_init_helper(root);
    return 0;
}

int trie_insert(trie_node_t *root, const char *word, size_t len) {
    if(root == NULL || word == NULL) {
        return EINVAL;
    }

    trie_node_t *cur = root;
    for(size_t i = 0; i < len; ++i){
        int idx = CHAR_TO_INDEX(word[i]);
        if(idx < 0 || idx >= ALPHABET_SIZE) {
            return EINVAL;
        }

        if(cur->child[idx] == NULL){
            cur->child[idx] = malloc(sizeof(trie_node_t));
            if(cur->child[idx] == NULL) {
                return ENOMEM;
            }
            node_init_helper(cur->child[idx]);
        }
        cur = cur->child[idx];
    }
    cur->is_end_of_word = true;
    return 0;
}

bool trie_search(trie_node_t *root, const char *word, size_t len) {
    if(root == NULL || word == NULL) {
        return false;
    }

    trie_node_t *cur = root;
    for(size_t i = 0; i < len; ++i){
        int idx = CHAR_TO_INDEX(word[i]);
        if(!IS_VALID_INDEX(idx) || cur->child[idx] == NULL) {
            return false;
        }

        cur = cur->child[idx];
    }
    return cur->is_end_of_word;
}

int trie_match_prefix(trie_node_t *root, const char *word, size_t len) {
    if(root == NULL || word == NULL) {
        return -1;
    }

    int match_len = 0;
    trie_node_t *cur = root;
    for(size_t i = 0; i < len; ++i){
        int idx = CHAR_TO_INDEX(word[i]);
        if(!IS_VALID_INDEX(idx)) {
            return -1;
        }

        if(cur->child[idx] == NULL){
            return match_len;
        }

        cur = cur->child[idx];
        ++match_len;
    }
    return match_len;
}

void trie_free_child(trie_node_t *node) {
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->child[i] != NULL) {
            trie_free_child(node->child[i]);
            free(node->child[i]);
        }
    }
    return;
}

void trie_free(trie_node_t *root) {
    if (root == NULL) {
        return;
    }

    trie_free_child(root);
    free(root);
    return;
}


