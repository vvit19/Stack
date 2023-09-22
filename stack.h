#ifndef STACK_H
#define STACK_H

typedef int elem_t;
#define EL_SPECIFICATOR "%d"

const int MIN_CAPACITY = 5;
const int GARBAGE = 696969;

struct stack
{
    elem_t* data;
    int capacity;
    int size;
};

void stack_ctor(stack* stk);
void stack_push(stack* stk, elem_t value);
void stack_pop(stack* stk);
void stack_dtor(stack* stk);

#endif