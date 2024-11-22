#include <stdlib.h>
#include <stdio.h>
#include <complex.h>
#include <math.h>
#include <fourier.h>

//Le o arquivo binario e armazena no vetor signal
unsigned char *get_audio(char *FileName, unsigned int *SampleSize){

    unsigned char byte, *signal = NULL;
    FILE *audio;


    *SampleSize = 0;
    audio = fopen(FileName, "r");
    while(!feof(audio)){
        *SampleSize += 1;
        signal = (unsigned char*)realloc(signal, *SampleSize*sizeof(unsigned char));
        fread(&byte, 1,sizeof(unsigned char),audio);
        signal[*SampleSize-1] = byte;
    }
    *SampleSize -= 1;
    fclose(audio);
    
    return signal;
}

//calcula os coeficientes da transformada
double complex *fourier_transform(unsigned char *signal,unsigned int N){

    int k, t;
    double complex *coeficients, Ck;
    double E,x;
    coeficients = (double complex*)malloc(((N/2)+1)*sizeof(double complex));

    for(k = 0;k < N/2 +1; k++){
        Ck = 0 + 0 * I;
        for(t = 0; t < N; t++){
            x = (2 * M_PI * k * t)/N;
            Ck += signal[t] * cos(x);
            Ck += (signal[t] * sin(x)) * I;
        }
        E = 2;
        if(k == 0) E = 1;
        E = E/N;
        coeficients[k] = (E * creal(Ck)) - ((E * cimag(Ck))*I);
    }
    
    return coeficients;
}
//calcula a magnitude dos coeficientes
float *magnitude(double complex *coeficients, unsigned int N){

    int i;
    float *result;
    result = (float*)malloc(((N/2)+1)*sizeof(float));

    for(i = 0;i < ((N/2)+1); i++){
        result[i] = cabs(coeficients[i]);
    }

    return result;
}

// Gera os novos coeficientes para a transformada inversa
double complex *cBiggest(float *magnVector, int C, unsigned int N, double complex *coeficient){

    int *pos, i,j, counter;
    float *auxVet, elemAux;
    double complex *newCoeficient;
    newCoeficient = (double complex*)calloc(((N/2)+1),sizeof(double complex));
    auxVet = (float*)malloc(((N/2)+1)*sizeof(float));
    pos = (int*)malloc(C*sizeof(int));

    for(i=0;i<((N/2)+1);i++){
        auxVet[i] = magnVector[i];
    }

    for(i=1;i<((N/2)+1);i++){
        elemAux = auxVet[i];
        for(j=i;j>0 && elemAux > auxVet[j-1];j--){
            auxVet[j] = auxVet[j-1];
        }
        auxVet[j] = elemAux;
    }

    counter = 0;
    for(i=0;i<((N/2)+1) && counter != C;i++){
        for(j=0;j<((N/2)+1) && counter != C; j++){
            if(auxVet[counter] == magnVector[j]){
                pos[counter] = j;
                counter++;
            }
        }
    }

    for(i=0;i<C;i++){
        newCoeficient[pos[i]] = coeficient[pos[i]];
        printf("%d ", (int)(auxVet[i]));
    }
    printf("\n");

    return newCoeficient;
}

//Aplica a transformada inversa nos novos coeficientes
unsigned char *inverse_fourier_transform(double complex *coeficients, unsigned int N){

    int k, t;
    unsigned char *result;
    double x;
    double complex sum;
    result = (unsigned char*)calloc(N,sizeof(unsigned char));

    for(t = 0; t < N; t++){
        sum = 0;
        for(k = 0;k < (N/2) +1; k++){
            x = (2 * M_PI * k * t)/N;
            sum += (cos(x) + sin(x) * I)*coeficients[k];
        }
        result[t] = round(creal(sum));
        printf("%d\n", result[t]);
    }

    return result;
}

//Escreve o audio transformado em um arquivo binário.Não utilizado no run.codes
void export_audio(char *FileName, unsigned char *audio, unsigned int N){
    FILE *fp;

    fp = fopen(FileName, "w");
    fwrite(audio, sizeof(unsigned char), N, fp);
}