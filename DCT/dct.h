#ifndef _DCT_
#define _DCT_
/*Escopo das funcoes definidas em dct.c
A funcao DCT recebe como parametro o vetor a ser transformado(definido como constante para evitar 
quaisquer modificacoes no vetor original) e o tamanho do mesmo.Seu retorno contem um ponteiro tipo float
para o vetor transformado*/
float *DCT(float * const, unsigned int);
//A funcao printVector por sua vez apenas coloca os valores contidos no vetor na saida padrao(stdout)
void printVector(float * const, unsigned int);

#endif