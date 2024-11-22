#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <trie.h>
#include <stack.h>
#include <huffman.h>

//Busca em profundidade para criar a cadeia de bits de uma letra
//Utiliza um primo para referenciar a tabela hash que contera os caracteres codificados
void DFS(TRIE_NODE *tree_node,STACK *stack,HUFFMAN_ELEM **table,int prime){
    int pos;
    if(tree_node && tree_node->node_type){
        pos = ((int)tree_node->letter)%prime;
        table[pos] = (HUFFMAN_ELEM*)malloc(sizeof(HUFFMAN_ELEM));
        table[pos]->code = stack_to_result(stack);
        table[pos]->letter = tree_node->letter;
        table[pos]->bit_size = stack->size;
    }
    else{
        if(tree_node && tree_node->left){
            stack_push(stack,0);
            DFS(tree_node->left,stack,table,prime);
        }
        if(tree_node && tree_node->right){
            stack_push(stack,1);
            DFS(tree_node->right,stack,table,prime);
        }
    }
    stack_pop(stack);
}

//Gera uma tabela hash para referenciar os caracteres e suas respectivas codificacoes
HUFFMAN_ELEM **huffman_table_generate(TRIE *tree,int prime){

    STACK *stack = create_stack();
    HUFFMAN_ELEM **table = (HUFFMAN_ELEM**)calloc(prime,sizeof(HUFFMAN_ELEM*));
    DFS(tree->root,stack,table,prime);
    stack_free(&stack);

    return table;
}

//Gera o texto codificado em bits
unsigned char *huffman_encode(HUFFMAN_ELEM **table, char *text_file, int text_size, int *encoded_text_size,int prime){
    unsigned char *encoded_text = NULL;
    int pos,i,counter = 0;

    for(i=0;i<text_size;i++){
        pos = ((int)text_file[i])%prime;
        counter += table[pos]->bit_size;
        encoded_text = (unsigned char*)realloc(encoded_text,counter*sizeof(unsigned char));
        memcpy(encoded_text+(counter-table[pos]->bit_size),table[pos]->code,table[pos]->bit_size);
    }

    *encoded_text_size = counter;
    return encoded_text;
}

//Decodifica a cadeia de binarios para texto puro
char *huffman_decode(TRIE *tree, unsigned char *encoded_text, int encoded_size, int *decoded_size){
    TRIE_NODE *p;
    char *decoded_text = NULL;
    int counter = 0, i = 0;

    while(i<encoded_size){
        p = tree->root;
        while(!p->node_type){
            if(encoded_text[i]) p = p->right;
            else p = p->left;
            i++;
        }
        counter++;
        decoded_text = (char*)realloc(decoded_text,sizeof(char)*counter);
        decoded_text[counter-1] = p->letter;
    }

    *decoded_size = counter;
    return decoded_text;
}

void huffman_free_table(HUFFMAN_ELEM **table, int size){
    int i;
    for(i=0;i<size;i++){
        if(table[i]){
            free(table[i]->code);
            free(table[i]);
        }
    }
}

