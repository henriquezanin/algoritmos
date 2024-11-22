#include <stdlib.h>
#include <stdio.h>
#include <complex.h>
#include <fourier.h>

//Henrique Gomes Zanin
//nUSP: 10441321

int main (int argc, char **argv){

    unsigned char *signal, *new_audio;
    unsigned int SampleSize;
    float *magnVector;
    double complex *coeficient, *newCoeficients;
    char *FileName;
    int C, i, nRelevCoef = 0;

    FileName = (char*)malloc(15*sizeof(char*));
    scanf("%s", FileName);
    scanf("%d", &C);

    //Le o arquivo binario e armazena no vetor signal
    signal = get_audio(FileName, &SampleSize);
    //calcula os coeficientes da transformada
    coeficient = fourier_transform(signal, SampleSize);
    //calcula a magnitude dos coeficientes
    magnVector = magnitude(coeficient, SampleSize);

   for(i = 0; i < SampleSize/2+1; i++){
        if(magnVector[i] > 0.1) nRelevCoef++;
    }

    printf("%d\n", SampleSize);
    printf("%d\n", nRelevCoef);

    // Gera os novos coeficientes para a transformada inversa
    newCoeficients = cBiggest(magnVector, C, SampleSize, coeficient);
    //Aplica a transformada inversa nos novos coeficientes
    new_audio = inverse_fourier_transform(newCoeficients,SampleSize);


    free(signal);
    free(new_audio);
    free(magnVector);
    free(coeficient);
    free(newCoeficients);
    free(FileName);

    return 0;
}