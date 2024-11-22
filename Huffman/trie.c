#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dyn_list.h> 
#include <trie.h>

//Inicializa a arvore que contera os nos que representam os caracteres
TRIE *trie_create(){
    return (TRIE*)calloc(1,sizeof(TRIE));
}

//Cria os nos que conterao os caracteres e os ordena da menor frequencia para a maior
TRIE_NODE **__build_frequecy_node(int *frequency, int size, int *array_size){
    int j,i,counter = 0;
    TRIE_NODE **node_array = NULL;
    TRIE_NODE *aux;

    for(i=0;i<size;i++){
        if(frequency[i] > 0){
            node_array = (TRIE_NODE**)realloc(node_array,(counter+1)*sizeof(TRIE_NODE*));
            aux = (TRIE_NODE*)calloc(1,sizeof(TRIE_NODE));
            aux->freq = frequency[i];
            aux->letter = i;
            aux->node_type = 1;
            node_array[counter] = aux;
            counter++;
        }
    }

    for(i=counter;i>=0;i--){
        for(j=0;j<i-1;j++){
            if(node_array[j]->freq > node_array[j+1]->freq){
                aux = node_array[j];
                node_array[j] = node_array[j+1];
                node_array[j+1] = aux;
            }
        }
    }

    *array_size = counter;

    return node_array;
}

//Cria a arvore com todos os caracteres e suas respectivas frequencias
//Utiliza uma lista encadeada para remover os dois primeiros elementos e concatena-los
//em outro no. O novo no eh adicionado a lista e ordenado de acordo com sua frequencia 
//para que possa ser recuperado posteriormente
int build_trie(TRIE *tree,int *frequency, int size){
    if(!tree) return INVALID_TREE;

    int i,counter;
    TRIE_NODE **node_array = __build_frequecy_node(frequency,size,&counter);
    TRIE_NODE *new_node;
    DYN_LIST *dyn_list = dyn_list_create();

    for(i=0;i<counter;i++){
            dyn_list_add(dyn_list,(void*)node_array[i],node_array[i]->freq);
    }

    while(dyn_list->size > 1){
        new_node = (TRIE_NODE*)calloc(1,sizeof(TRIE_NODE));
        new_node->right = (TRIE_NODE*)dyn_list_remove_first(dyn_list);
        new_node->left = (TRIE_NODE*)dyn_list_remove_first(dyn_list);
        new_node->freq = (new_node->left->freq) + (new_node->right->freq);
        dyn_list_add(dyn_list,(void*)new_node,new_node->freq);
    }
    tree->root = (TRIE_NODE*)dyn_list_remove_first(dyn_list);

    return 0;
}

void free_trie(TRIE_NODE *node){
    if(node->left) free_trie(node->left);
    if(node->right) free_trie(node->right);
    free(node);
}