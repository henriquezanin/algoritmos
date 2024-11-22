#ifndef __ARRAY_LIST__
#define __ARRAY_LIST__

#define SUCCESS 0
#define INVALID_ARRAY_LIST 4
#define INVALID_POS 5

struct item{
    int value;
    unsigned int key;
};

struct no{
   struct item **elem;
   int array_size;
   int atual_size;
   struct no *next; 
};

struct array_list{
    int size;
    int static_size;
    int real_size;
    struct no *first, *last;
};

typedef struct item ITEM;
typedef struct no NO;
typedef struct array_list ARRAY_LIST;

ARRAY_LIST* ArrayList();
int ArrayList_add(ARRAY_LIST*, int, int);
ITEM *ArrayList_remove(ARRAY_LIST *, int);
int ArrayList_print(ARRAY_LIST*);
int ArrayList_size(ARRAY_LIST *);
int ArrayList_isEmpty(ARRAY_LIST *);
int ArrayList_free(ARRAY_LIST **);
int ArrayList_contain(ARRAY_LIST *, int);
ITEM *ArrayList_get(ARRAY_LIST *,int);
int ArrayList_index(ARRAY_LIST *, int);
int ArrayList_set(ARRAY_LIST *, int, int, int);
ARRAY_LIST *ArrayList_sub(ARRAY_LIST *, int, int);
int ArrayList_OnlyListFree(ARRAY_LIST **);


#endif