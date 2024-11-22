#include <stdio.h>
#include <stdlib.h>
#include <hash.h>

int main(int argc, char **argv){

    char **words;
    int size,i, prime;
    HASH_TABLE *hashTable;
    char *input = (char*)calloc(7,sizeof(char));

    scanf("%s", input);
    //Armazena as palavras do arquivo de entrada
    words = get_data(input, &size);

    prime = 8000009;
    //Inicializa a tabela hash
    hashTable = create_hash_table(prime);
    
    //Insere todas as palavras na tabela hash
    for(i=0;i<size;i++){
        hash_add(hashTable,prime,words[i]);
    }

    //Escreve no stdout todos os possiveis resultados 
    print_biggest(hashTable);

    free(input);
    for(i=0;i<size;i++){
        free(words[i]);
    }
    free(words);
    free_hash(hashTable);

    return 0;
}