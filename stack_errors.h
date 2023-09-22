#ifndef STACK_ERRORS_H
#define STACK_ERRORS_H

#include "stack.h"

const int ERRORS_NUM = 4;

enum stack_errors
{
    STACK_NULLPTR = 1,
    NEGATIVE_SIZE = 2,
    NEGATIVE_CAPACITY = 4,
    DATARRAY_NULLPTR = 8
};  

#define DUMP_STRUCTURE \
    "stack [%X] %s called from %s(%d) %s()\n" \
    "size = %d\n" \
    "capacity = %d\n" \
    "data[%X]\n\n" \
    
#define LOG_FILE "log.txt"

#define STACK_DUMP(x) stack_dump(x, #x, __PRETTY_FUNCTION__, __FILE__, __LINE__)

void stack_verify(stack* stk);
void stack_dump(stack* stk, const char* variable_name, const char* function_name, 
                        const char* program_name, int line);

#endif 