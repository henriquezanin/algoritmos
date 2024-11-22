#include <stdlib.h>
#include <stdio.h>
#include <dyn_list.h>

//Cria a lista
DYN_LIST *dyn_list_create(){
    return (DYN_LIST*)calloc(1,sizeof(DYN_LIST));
}

//Adiciona um elemento na lista de forma ordenada
int dyn_list_add(DYN_LIST *list,void *elem,int key){
    if(!list) return -1;

    DYN_LIST_ELEM *new = (DYN_LIST_ELEM*)calloc(1,sizeof(DYN_LIST_ELEM));
    DYN_LIST_ELEM **p = &list->first;

    while(*p && (*p)->key < key) p = &(*p)->next;
    new->elem = elem;
    new->key = key;
    new->next = *p;
    *p = new;

    list->size++;

    return 0;
}

//Remove um elemento da lista
int dyn_list_remove(DYN_LIST *list, int key){
    if(!list) return -1;

    DYN_LIST_ELEM *p = list->first;
    DYN_LIST_ELEM *aux;

    while(p && p->key < key) p = p->next;

    if(p){
        aux = p->next;
        p->next = aux->next;
        free(p);
        list->size--;
    }

    return 0;
}

//Remove o primeiro elemento da lista
void *dyn_list_remove_first(DYN_LIST *list){
    if(!list) return NULL;

    DYN_LIST_ELEM *p = list->first;
    void *elem = NULL;

    if(p){
        elem = p->elem;
        list->first = p->next;
        free(p);
        list->size--;
    }

    return elem;
}