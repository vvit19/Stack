#ifndef STACK_ERRORS_H
#define STACK_ERRORS_H

#include "stack.h"

const int ERRORS_NUM = 5;

enum stack_errors
{
    STACK_NULLPTR = 1,
    NEGATIVE_SIZE = 2,
    NEGATIVE_CAPACITY = 4,
    DATARRAY_NULLPTR = 8,
    SIZE_BIGGER_THAN_CAPACITY = 16
};  

void stack_verify(stack* stk);

#ifdef _DEBUG

#define STACK_DUMP(stk) stack_dump(stk)
void stack_dump(stack* stk);

#endif

#endif 