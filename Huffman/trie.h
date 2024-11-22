#ifndef __TRIE__
#define __TRIE__

#define INVALID_TREE -1

//No da arvore
struct trie_node{
    int freq;
    unsigned char node_type;
    char letter;
    struct trie_node *left, *right;
};

//Descritor da arvore
struct trie{
    struct trie_node *root;
};

typedef struct trie TRIE;
typedef struct trie_node TRIE_NODE;

TRIE *trie_create();
int build_trie(TRIE *, int *,int);
void free_trie(TRIE_NODE *);

#endif