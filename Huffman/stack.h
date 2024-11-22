#ifndef __STACK__
#define __STACK__

//elemeto da pilha
struct stack_elem{
    int value;
    struct stack_elem *next;
};

//Estrutura da pilha
struct stack{
    int size;
    struct stack_elem *top;
};

typedef struct stack STACK;
typedef struct stack_elem STACK_ELEM;

STACK *create_stack();
int stack_push(STACK*,int);
int stack_pop(STACK*);
int stack_free(STACK**);
unsigned char *stack_to_result(STACK*);

#endif