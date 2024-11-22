#ifndef __HUFFMAN__
#define __HUFFMAN__
#include <trie.h>

//Elemento que contera o codigo binario, a letra e o tamanho em bits
struct huffman_elem{
    int bit_size;
    char letter;
    unsigned char *code;
};

typedef struct huffman_elem HUFFMAN_ELEM;

HUFFMAN_ELEM **huffman_table_generate(TRIE*,int);
unsigned char *huffman_encode(HUFFMAN_ELEM **, char *, int, int *,int);
char *huffman_decode(TRIE *, unsigned char *, int, int *);
void huffman_free_table(HUFFMAN_ELEM **, int);
#endif