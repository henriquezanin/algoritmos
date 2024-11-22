#include <stdlib.h>
#include <stdio.h>
#include <huffman.h>

#define ASCII_SIZE 127
#define PRIME 97

int main(int argc, char **argv){

    //Inicializa uma tabela hash para acomodar a contagem de frequencias
    int *frequency = (int*)calloc(ASCII_SIZE,sizeof(int));
    //Demais inicializacoes
    int i,encoded_text_size,decoded_text_size,text_size = 0;
    char letter,*decoded_text,*text_file = NULL;
    unsigned char *encoded_text;
    HUFFMAN_ELEM **table;
    //Cria a arvore usada para a codificacao e decodificacao
    TRIE *trie = trie_create();

    //Le os caracteres do stdin, realiza a contagem de frequencia e armazena o texto
    //em um vetor dinamicamente alocado
    scanf("%c",&letter);
    while(!feof(stdin)){
        if((int)letter != 10){
            text_size++;
            text_file = (char*)realloc(text_file,sizeof(char)*text_size);
            frequency[(int)letter] += 1;
            text_file[text_size-1] = letter;
        }
        scanf("%c",&letter);
    }

    //Cria a arvore de frequancias
    build_trie(trie,frequency,ASCII_SIZE);
    free(frequency);
    //Gera a tabela hash de caracteres codificados
    table = huffman_table_generate(trie,PRIME);
    
    printf("%d\n",text_size);
    //Codifica o texto para binario, o primo eh usado para referenciar a tabela hash inicializada
    //pela funcao "huffman_table_generate"
    encoded_text = huffman_encode(table,text_file,text_size,&encoded_text_size,PRIME);
    
    //Decodifica o texto gerado pela funcao acima
    decoded_text = huffman_decode(trie,encoded_text,encoded_text_size,&decoded_text_size);

    encoded_text_size += 8-(encoded_text_size%8);
    printf("%d\n",(encoded_text_size/8)+1);

    for(i=0;i<decoded_text_size;i++){
        printf("%c",decoded_text[i]);
    }
    printf("\n");

    free(encoded_text);
    free(decoded_text);
    free(text_file);
    huffman_free_table(table,PRIME);
    free_trie(trie->root);

    return 0;
}