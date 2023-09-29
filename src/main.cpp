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

    for (int i = 0; i < 5; i++)
    {
        stack_pop(&stk);
    }

    stack_dtor(&stk);
}

