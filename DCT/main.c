#include <stdio.h>
#include <stdlib.h>
#include <dct.h>

/* Leitura dos dados passados pela entrada padrao(stdin) e utilizacao das funcoes DTC e printVector
previamente definidas no arquivo dct.c
*/
int main(int argc, char **argv){
    unsigned int vectorSize, i;
    float *IMG, *TransformIMG;

    fscanf(stdin, "%d", &vectorSize);
    IMG = (float*)malloc(vectorSize*sizeof(float));

    for(i=0;i<vectorSize;i++){
        fscanf(stdin,"%f",&IMG[i]);
    }

    TransformIMG = DCT(IMG, vectorSize);

    printVector(TransformIMG, vectorSize);

    free(IMG);
    free(TransformIMG);
}