#ifndef __HASH__
#define __HASH__

//Define a estrutura para um item da tabela hash
struct hash_item{
    int size;
    char **anagram;
};

//Define a estrutura da tabela hash
struct hash_table{
    int size;
    int biggest;
    struct hash_item **elem;
};

typedef struct hash_item HASH_ITEM;
typedef struct hash_table HASH_TABLE;

char **get_data(char *, int*);
unsigned long int hash_func(char *,int);
HASH_TABLE *create_hash_table(int);
int hash_add(HASH_TABLE *,int, char*);
void print_biggest(HASH_TABLE*);
void free_hash(HASH_TABLE *);

#endif