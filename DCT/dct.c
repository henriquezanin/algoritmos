#include <stdlib.h>
#include <stdio.h>
#include <dct.h>
#include <math.h>

//Retorna um ponteiro tipo float para o vetor transformado
float *DCT(float * const IMGvector, unsigned int size){
    unsigned int i,j;
    float *TransformIMG;

    TransformIMG = (float*)calloc(size,sizeof(float));

    //Aplicacao da DCT-II
    for(i=0;i<size;i++){
        for(j=0;j<size;j++){
            TransformIMG[i] += IMGvector[j]*cos((M_PI/size)*(j+0.5)*i);
        }
    }
    return TransformIMG;
}

void printVector(float *const Vector, unsigned int size){
    unsigned int i;

    for(i=0;i<size;i++){
        printf("%f\n", Vector[i]);
    }
}