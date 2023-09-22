#include <cstdio>

#include "stack.h"

int main()
{
    stack stk = {};

    stack_ctor(&stk);
    stack_push(&stk, 3);
    stack_push(&stk, -1);
    stack_push(&stk, 69);
    stack_pop (&stk);
    stack_push(&stk, 12);
    stack_push(&stk, 6);
    stack_push(&stk, -69);
    stack_push(&stk, 12);
    stack_push(&stk, 9);
    for (int i = 0; i < 100; i++) stack_pop(&stk);    
    stack_dtor(&stk);
}

