#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <hash.h>

//Leitura do arquivo texto
char **get_data(char *Filename, int *size){

    char** words = NULL;
    char *aux = (char*)calloc(20,sizeof(char));
    FILE *fp;

    fp = fopen(Filename, "r");
    *size = 0; 
    while(!feof(fp)){
        (*size)++;
        words = (char**)realloc(words,(*size)*sizeof(char*));
        fscanf(fp,"%s",aux);
        words[(*size)-1] = (char*)calloc(strlen(aux)+1,sizeof(char));
        strcpy(words[(*size)-1], aux);
    }
    free(aux);

    return words;
}

//Transforma a string em um valor numerico para ser usado como indice da tabela hash
unsigned long int hash_func(char *string,int prime){
    int *prime_numbers = (int*)malloc(26*sizeof(int));
    unsigned long int i, hash = 1;

    prime_numbers[0] = 2;
    prime_numbers[1] = 3;
    prime_numbers[2] = 5;
    prime_numbers[3] = 7; 
    prime_numbers[4] = 11; 
    prime_numbers[5] = 13; 
    prime_numbers[6] = 17; 
    prime_numbers[7] = 19; 
    prime_numbers[8] = 23; 
    prime_numbers[9] = 29; 
    prime_numbers[10] = 31;   
    prime_numbers[11] = 37; 
    prime_numbers[12] = 41;  
    prime_numbers[13] = 43;  
    prime_numbers[14] = 47;  
    prime_numbers[15] = 53;  
    prime_numbers[16] = 59;  
    prime_numbers[17] = 61;  
    prime_numbers[18] = 67;  
    prime_numbers[19] = 71;
    prime_numbers[20] = 73; 
    prime_numbers[21] = 79; 
    prime_numbers[22] = 83; 
    prime_numbers[23] = 89; 
    prime_numbers[24] = 97;
    prime_numbers[25] = 101; 

    for(i=0;i<strlen(string);i++){
        hash = hash * (string[i]*(prime_numbers[((int)string[i])-97]))%prime;
    }

    return hash;
}

//Inicializa a tabela hash
HASH_TABLE *create_hash_table(int size){
    HASH_TABLE *hashTable = (HASH_TABLE*)calloc(1,sizeof(HASH_TABLE));
    hashTable->size = size;
    hashTable->elem = (HASH_ITEM**)calloc(size, sizeof(HASH_ITEM*));

    return hashTable;
}

//Adiciona uma palavra na tabela hash
int hash_add(HASH_TABLE *hash,int prime,char *word){

    unsigned long int hash_value = hash_func(word, prime);
    unsigned long int pos = hash_value;

    if(!hash->elem[pos]){
        hash->elem[pos] = (HASH_ITEM*)calloc(1,sizeof(HASH_ITEM));
        hash->elem[pos]->anagram = (char**)malloc(sizeof(char*));
        hash->elem[pos]->anagram[hash->elem[pos]->size] = (char*)malloc(strlen(word)*sizeof(char));
        hash->elem[pos]->anagram[hash->elem[pos]->size] = word;
        hash->elem[pos]->size++;
    }
    else{
        hash->elem[pos]->anagram = (char**)realloc(hash->elem[pos]->anagram,(hash->elem[pos]->size+1)*sizeof(char*));
        hash->elem[pos]->anagram[hash->elem[pos]->size] = word;
        hash->elem[pos]->size++;
    }

    if(hash->elem[pos]->size > hash->biggest) hash->biggest = hash->elem[pos]->size;

    return 0;
}

//Escreve no stdout o maior conjunto de anagramas em ordem alfabetica
void print_biggest(HASH_TABLE *hash){
    int i,j=0, counter;
    HASH_ITEM **Item, *aux;

    for(i=0;i<hash->size;i++){
        if(hash->elem[i] && hash->elem[i]->size == hash->biggest) j++;
    }

    Item = (HASH_ITEM**)malloc(j*sizeof(HASH_ITEM));

    printf("%d\n", j);
    counter = 0;
    for(i=0;i<hash->size;i++){
        if(hash->elem[i] && hash->elem[i]->size == hash->biggest){
            Item[counter++] = hash->elem[i];
        }
    }
    for(i=counter;i>0;i--){
        for(j=0;j<i-1;j++){
            if((strcmp(Item[j]->anagram[0],Item[j+1]->anagram[0]))>0){
                aux = Item[j];
                Item[j] = Item[j+1];
                Item[j+1] = aux;
            }
        }
    }
    for(i=0;i<counter;i++){
        for(j=0;j<hash->biggest-1;j++){
            printf("%s, ",Item[i]->anagram[j]);
        }
        printf("%s\n",Item[i]->anagram[j]);
    }

}

//Libera toda a memoria alocada para a tabela hash
void free_hash(HASH_TABLE *hash){
    int i;

    for(i=0;i<hash->size;i++){
        if(hash->elem[i]){
            free(hash->elem[i]);
        }
    }
    free(hash);
}