#include <stdio.h>
#include <stdlib.h>
#include "bitwise.h"

//Bitwise AND function
void bitwise_add(unsigned int value1, unsigned int value2){
    printf("%d\n", value1 & value2);
}

//Bitwise OR function
void bitwise_or(unsigned int value1, unsigned int value2){
    printf("%d\n", value1 | value2);
}

//Bitwise XOR function
void bitwise_xor(unsigned int value1, unsigned int value2){
    printf("%d\n", value1 ^ value2);
}

//Bitwise COMPLEMENT function
void bitwise_complement(unsigned int value){
    printf("%d\n", ~value);
}

//Bitwise shift left or right function
void bitwise_shift(unsigned int value, char option, unsigned int nBits){
    if(option == 'l'){
        printf("%d\n", value << nBits);
    }
    else if(option = 'r'){
        printf("%d\n", value >> nBits);
    }
    else{
        puts("Invalid option for bitwise shift function");
    }
}
