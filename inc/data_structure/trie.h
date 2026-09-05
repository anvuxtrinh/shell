#pragma once

#include <stdbool.h>

#define ALPHABET_SIZE 26

typedef struct trie_node {
    struct trie_node *child[ALPHABET_SIZE];
    bool is_end_of_word;
} trie_node_t;

int trie_init(trie_node_t *root);
int trie_insert(trie_node_t *root, const char *word, size_t len);
bool trie_search(trie_node_t *root, const char *word, size_t len);
int trie_match_prefix(trie_node_t *root, const char *word, size_t len);
void trie_free_child(trie_node_t *node);
void trie_free(trie_node_t *root);  