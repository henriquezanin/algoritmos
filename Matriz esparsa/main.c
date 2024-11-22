#include <stdlib.h>
#include <stdio.h>
#include <sparse.h>

int main(int argc, char **argv){

    char MathOperation;
    int input1, input2, input3;
    SPARSE *matrix1, *matrix2, *resultant;

    scanf("%c", &MathOperation);
    scanf("%d %d %d", &input1, &input2, &input3);

    //Cria a primeira matriz esparsa
    matrix1 = sparse_create(input2,input3);

    do{
        scanf("%d %d %d", &input1, &input2, &input3);
        if(input1 != -1){
            //Insere os elementos na primeira matriz esparsa
            sparse_put(matrix1,input1,input2,input3);
        }
    }while(input1 != -1);

    //Cria a segunda matriz esparsa
    matrix2 = sparse_create(input2,input3);

    while(!feof(stdin)){
        scanf("%d %d %d", &input1, &input2, &input3);
        //Insere os elementos na segunda matriz esparsa
        sparse_put(matrix2,input1,input2,input3);
    }

    //Realiza as operacoes solicidatas entre as duas matrizes, os resultados sao adicionados
    //na matriz resultant
    if(MathOperation == 'A') resultant = sparse_sum(matrix1,matrix2);
    else if(MathOperation == 'M') resultant = sparse_mult(matrix1,matrix2);
    else printf("Invalid option!");
    
    sparse_print(resultant);
    sparse_free(resultant);
    sparse_free(matrix1);
    sparse_free(matrix2);

    return 0;
}