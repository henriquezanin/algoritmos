#ifndef __SPARSE__
#define __SPARSE__

#define SUCCCESS 0
#define INVALID_SPARSE -1
#define INVALID_POSITION -2

struct sparse_elem{
    int row, col, value;
    struct sparse_elem *right, *botton;
};

struct sparse{
    int nrow, ncol, constant;
    struct sparse_elem **row, **col;
};

typedef struct sparse SPARSE;
typedef struct sparse_elem SPARSE_ELEM;

SPARSE *sparse_create(int,int);
int sparse_put(SPARSE *,int,int,int);
int sparse_get(SPARSE *, int, int);
SPARSE *sparse_sum(SPARSE*,SPARSE*);
SPARSE *sparse_mult(SPARSE *, SPARSE *);
int sparse_free(SPARSE *);
void sparse_print(SPARSE *);

#endif