#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arraylist.h>

//Cria o descritor da lista simplesmente encadeada
ARRAY_LIST *ArrayList(){

    ARRAY_LIST *list;
    list = (ARRAY_LIST*)calloc(1,sizeof(ARRAY_LIST));
    
    return list;
}

//Adiciona um no na lista encadeada na posicao desejada
int add_node(ARRAY_LIST *list, unsigned int pos){
    if(!list) return INVALID_ARRAY_LIST;
    if(pos < 0 || pos > list->size) return INVALID_POS;

    
    unsigned int counter = 0;
    NO *new = (NO*)calloc(1, sizeof(NO));
    NO **p = &list->first;

    while(counter++ < pos && (p = &(*p)->next));

    new->next = *p;
    *p = new;

    if(pos == list->size) list->last = new;

    list->size++;

    return SUCCESS;
}

//remove um no da lista encadeada em uma dada posicao
int remove_node(ARRAY_LIST *list, unsigned int pos){
    if(!list) return INVALID_ARRAY_LIST;
    if(pos < 0 || pos > list->size) return INVALID_POS;

    unsigned int counter = 0;
    NO *p, *prev;
    
    prev = NULL;
    p = list->first;

    while(counter++ < pos){
        prev = p;
        p = p->next;
    }
    if(prev){
         prev->next = p->next;
         if(list->last == p) list->last = prev;
    }
    else list->first = p->next;

    free(p);
    list->size--;

    return SUCCESS;
}

//Cria uma struct do tipo ITEM para acomodar a chave e o valor
ITEM *create_item(int key, int value){
    ITEM *elem;
    elem = (ITEM*)malloc(sizeof(ITEM));
    elem->key = key;
    elem->value = value;

    return elem;
}

//Executa busca binaria dentro do vetor
int binary_search(ITEM **vector,int left, int right, int key){
    int middle;

    while(left <= right){
        middle = left + (right-left)/2;
        if(vector[middle]->key == key) return middle;
        if(vector[middle]->key < key) left = middle + 1;
        else right = middle - 1;
    }
    
    return -1;
}

//Adiciona o primeiro no e aloca um vetor de tamanho 10
int add_first_node(ARRAY_LIST *list){
    if(!list) return INVALID_ARRAY_LIST;

    add_node(list,0);
    list->first->array_size = 10;
    list->static_size = 10;
    list->first->elem = (ITEM**)calloc(10,sizeof(ITEM*));
    return SUCCESS;
}

//Adiciona o ultimo no e aloca um vetor com o dobro do tamanho do anterior
int add_last_node(ARRAY_LIST *list){
    if(!list) return INVALID_ARRAY_LIST;
    int size = list->last->array_size * 2;

    add_node(list,list->size);
    list->last->array_size = size;
    list->static_size += size;
    list->last->elem = (ITEM**)calloc(size,sizeof(ITEM*));
    return SUCCESS;
}

//Adiciona de forma ordenada um elemento no ArrayList
int ArrayList_add(ARRAY_LIST* list, int key, int value){

    int pos;
    ITEM *last_item, *next_last_item;

    if(!list) return INVALID_ARRAY_LIST;
    NO *p = list->first;
    if(!p){
        add_first_node(list);
        list->first->elem[0] = create_item(key,value);
        list->first->atual_size++;
        list->real_size++;
        return SUCCESS;
    }
    if(list->last->array_size == list->last->atual_size) add_last_node(list);

    while(p && (p->array_size == p->atual_size) && p->array_size > 0 && (p->elem[p->atual_size-1]->key < key)) p = p->next;

    for(pos=0;pos < p->atual_size && p->elem[pos]->key < key;pos++);

    if(p->array_size != p->atual_size){
        memmove(&(p->elem[pos+1]),&(p->elem[pos]),sizeof(ITEM*)*(p->atual_size-pos));
        p->elem[pos] = create_item(key,value);
        p->atual_size++;
        list->real_size++;
        return SUCCESS; 
    }

    last_item = p->elem[p->atual_size-1];
    memmove(&(p->elem[pos+1]),&(p->elem[pos]),sizeof(ITEM*)*(p->atual_size-pos));
    p->elem[pos] = create_item(key,value);
    p = p->next;


    while(p && p->atual_size == p->array_size){
        next_last_item = p->elem[p->array_size-1];
        memmove(&(p->elem[1]),&(p->elem[0]),(p->atual_size-1)*(sizeof(ITEM*)));
        p->elem[0] = last_item;
        last_item = next_last_item;
        p = p->next;
    }

       memmove(&(p->elem[1]),&(p->elem[0]),p->atual_size*(sizeof(ITEM*)));
       p->elem[0] = last_item;
       p->atual_size++;
       list->real_size++;

    return SUCCESS;
}

//Remove um elemento do ArrayList e o retorna para facilitar a desalocacao de memoria
ITEM *ArrayList_remove(ARRAY_LIST *list, int pos){
    if(!list) return NULL;

    int pos_array = pos;
    ITEM *elem;
    NO *p = list->first;

    if(pos > list->real_size) return NULL;

    while(p && pos_array >= p->array_size){
        pos_array -= p->array_size;
        p = p->next;
    }
    elem = p->elem[pos_array];
    memmove(&p->elem[pos_array],&p->elem[pos_array+1],(p->atual_size-pos_array-1)*sizeof(ITEM*));

    while(p->next){
        p->elem[p->array_size-1] = p->next->elem[0];
        p = p->next;
        memmove(&(p->elem[0]),&(p->elem[1]),(p->atual_size-1)*(sizeof(ITEM*)));
    }
    p->elem[p->atual_size-1] = NULL;
    p->atual_size--;
    list->real_size--;
    if(!list->last->atual_size) remove_node(list,list->size-1);

    return elem;
}

//Retorna a quantidade de elementos no ArrayList
int ArrayList_size(ARRAY_LIST *list){
    if(!list) return INVALID_ARRAY_LIST;
    return list->real_size;
}

//Escreve no stdout todos os elementos do ArrayList
int ArrayList_print(ARRAY_LIST *list){
    if(!list) return INVALID_ARRAY_LIST;

    int i, global;
    global = 0;
    NO *p = list->first;

    while(p){
        for(i = 0;i<p->atual_size;i++,global++){
            printf("%d: ", global);
            printf("%d/%d\n",p->elem[i]->key, p->elem[i]->value);
        }
        printf("\n\n");
        p = p->next;
    }

    return SUCCESS;
}

//Retorna 1 se o ArrayList estiver vaizio e 0 caso possua algum elemento
int ArrayList_isEmpty(ARRAY_LIST *list){
    if(!list) return INVALID_ARRAY_LIST;
    return !list->real_size;
}

//Libera a memoria ocupada por todos os elementos, vetores e a lista
int ArrayList_free(ARRAY_LIST **list){
    if(!(*list)) return INVALID_ARRAY_LIST;

    NO *aux, *p = (*list)->first;
    int i;

    while(p){
        aux = p;
        for(i = 0;i < p->atual_size; i++){
            free(p->elem[i]);
        }
        free(p->elem);
        p = p->next;
        free(aux);
    }

    free(*list);
    *list = NULL;

    return SUCCESS;
}

//Verifica pela chave se um elemento esta contido no ArrayList
int ArrayList_contain(ARRAY_LIST *list, int key){
    if(!list) return INVALID_ARRAY_LIST;
    int pos;

    NO *p = list->first;

    while(p && p->elem[p->atual_size-1]->key < key) p = p->next;

    if(p)
        pos = binary_search(p->elem,0,p->atual_size-1,key);
    else pos = -1;

    if(pos == -1) return 0;
    return 1;
}

//Retorna o elemento de uma dada posicao no ArrayList
ITEM *ArrayList_get(ARRAY_LIST *list, int pos){
    if(!list) return NULL;
    int pos_array = pos;
    ITEM *elem;
    NO *p = list->first;

    if(pos > list->real_size) return NULL;

    while(p && pos_array >= p->array_size){
        pos_array -= p->array_size;
        p = p->next;
    }
    elem = p->elem[pos_array];

return elem;
}

//Retorna o indice de um elemento
int ArrayList_index(ARRAY_LIST *list, int key){
    if(!list) return INVALID_ARRAY_LIST;

    int i, global;
    global = 0;
    NO *p = list->first;

    while(p && p->elem[p->atual_size-1]->key < key){
        global += p->atual_size;
        p = p->next;
    }
    
    if(p){
        for(i = 0; i < p->atual_size && p->elem[i]->key != key; i++);
        global += i;
    }
    else global = -1;

    return global;
}

//Remove o elemento de uma dada posicao e o insere de forma ordenada conforme sua nova chave
int ArrayList_set(ARRAY_LIST *list, int pos, int key, int value){
    if(!list) return INVALID_ARRAY_LIST;
    ITEM *elem;
    if(pos < list->real_size){
        elem = ArrayList_remove(list, pos);
        free(elem);
        ArrayList_add(list,key, value);
    }
    return SUCCESS;
}

//Cria uma copia da faixa de posicoes passada como parametro
//Retorna uma nova lista com os mesmos enderecos dos elementos
ARRAY_LIST *ArrayList_sub(ARRAY_LIST *list, int begin, int end){
    if(!list) return NULL;
    if(end > list->real_size || begin > list->real_size ) return NULL;
    ARRAY_LIST *new_list;
    ITEM *elem;
    int pos_begin = begin;

    new_list = ArrayList();

    if(end - begin) add_first_node(new_list);

    while(pos_begin <= end-1){
        elem = ArrayList_get(list,pos_begin);
        if(new_list->last->atual_size == new_list->last->array_size) add_last_node(new_list);
        new_list->last->elem[new_list->last->atual_size] = elem;
        new_list->real_size++;
        new_list->last->atual_size++;
        pos_begin++;
    }

    return new_list;
}

//Desaloca da memoria heap apenas a lista e os vetores que acomodam os elementos.
int ArrayList_OnlyListFree(ARRAY_LIST **list){
    if(!(*list)) return INVALID_ARRAY_LIST;

    NO *aux, *p = (*list)->first;

    while(p){
        aux = p;
        free(p->elem);
        p = p->next;
        free(aux);
    }

    free(*list);
    *list = NULL;

    return SUCCESS;
}