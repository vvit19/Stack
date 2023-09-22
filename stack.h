#ifndef STACK_H
#define STACK_H

typedef int elem_t;

const int MIN_CAPACITY = 5;
const int GARBAGE = 696969;

#ifdef _DEBUG

struct function_call_info
{
    const char* filename;
    const char* variable_name;
    const char* function_name;
    int line;
};

#endif

struct stack
{
    elem_t* data;
    int capacity;
    int size;

    #ifdef _DEBUG

    function_call_info func_call_info; 

    #endif
};


#ifdef _DEBUG

#define STACK_CTOR(stk) stack_ctor(stk, #stk, __PRETTY_FUNCTION__, __FILE__, __LINE__);

void stack_ctor(stack* stk, const char* variable_name, const char* function_name,
                const char* file_name, int line);

#else

#define STACK_CTOR(stk) stack_ctor(stk);
void stack_ctor(stack* stk);

#endif

void stack_push(stack* stk, elem_t value);
void stack_pop(stack* stk);
void stack_dtor(stack* stk);

#endif