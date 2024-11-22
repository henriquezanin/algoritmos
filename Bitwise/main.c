#include <stdio.h>
#include <stdlib.h>
#include "bitwise.h"

int main(int argc, char *argv[]){

    unsigned int value1, value2;

    //Get values from standard input
    fscanf(stdin, "%d %d", &value1, &value2);
    
    //execution of functions defined at bitwise.h
    bitwise_add(value1, value2);
    bitwise_or(value1, value2);
    bitwise_xor(value1, value2);
    bitwise_complement(value1);
    bitwise_complement(value2);
    bitwise_shift(value1, 'r', 2);
    bitwise_shift(value2, 'l', 2);

    return 0;
}