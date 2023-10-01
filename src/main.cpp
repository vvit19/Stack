#include <cstdio>

#include "../includes/stack.h"

int main()
{
    stack stk = {};

    STACK_CTOR(&stk);

    for (int i = 1; i <= 10; i++)
    {
        stack_push(&stk, i);
    }

    elem_t popped_value = 0;
    for (int i = 0; i < 5; i++)
    {
        stack_pop(&stk, &popped_value);
    }

    stack_dtor(&stk);
}

