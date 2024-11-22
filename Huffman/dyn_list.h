#ifndef __DYN_LIST__
#define __DYN_LIST__

//Elemento da lista
struct dyn_list_elem{
    void *elem;
    int key;
    struct dyn_list_elem *next;
};

//Descritor da lista
struct dyn_list{
    int size;
    struct dyn_list_elem *first;
};

typedef struct dyn_list DYN_LIST;
typedef struct dyn_list_elem DYN_LIST_ELEM;

DYN_LIST *dyn_list_create();
int dyn_list_add(DYN_LIST *,void *,int);
int dyn_list_remove(DYN_LIST *, int);
void *dyn_list_remove_first(DYN_LIST*);


#endif