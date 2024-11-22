#include <stdlib.h>
#include <stdio.h>
#include <stack.h>

//Inicializa a pilha
STACK *create_stack(){
    return (STACK*)calloc(1,sizeof(STACK));
}

//Insere dado na pilha
int stack_push(STACK *stack, int value){
    if(!stack) return -1;

    STACK_ELEM *new = (STACK_ELEM*)malloc(sizeof(STACK_ELEM));
    new->value = value;
    new->next = stack->top;
    stack->top = new;
    stack->size++;

    return 0;
}

//retira o dado da pilha e retorna seu valor
int stack_pop(STACK *stack){

    STACK_ELEM *elem;
    int value = -1;

    if(stack->top){
        elem = stack->top;
        value = elem->value;
        stack->top = elem->next;
        stack->size--;
        free(elem);
    }

    return value;
}

//Converte a pilha para um vetor
unsigned char *stack_to_result(STACK *stack){

    int i;
    unsigned char *array = (unsigned char*)malloc((stack->size)*sizeof(unsigned char));
    STACK_ELEM *elem;

    if(stack->top) elem = stack->top;

    for(i=stack->size-1;i>=0;i--){
        array[i] = elem->value;
        elem = elem->next;
    }
    return array;
}

//libera a pilha
int stack_free(STACK **stack){
    if(!(*stack)) return -1;

    int i;
    STACK_ELEM *elem;
    for(i=0;i<(*stack)->size;i++){
        elem = (*stack)->top;
        (*stack)->top = elem->next;
        free(elem);
    }
    free(*stack);

    return 0;
}