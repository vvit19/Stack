#ifndef STACK_H
#define STACK_H

typedef int elem_t;

const int MIN_CAPACITY = 5;
const int GARBAGE = 696969;

#ifdef _CANARY_PROTECTION

typedef unsigned long long canary_t;
const canary_t CANARY_CONST =  0xDEADBABE;

const int ERRORS_NUM = 7;

#else

const int ERRORS_NUM = 5;

#endif

enum stack_errors
{
    STACK_NULLPTR = 1,
    NEGATIVE_SIZE = 2,
    NEGATIVE_CAPACITY = 4,
    DATARRAY_NULLPTR = 8,
    SIZE_BIGGER_THAN_CAPACITY = 16,
    
    #ifdef _CANARY_PROTECTION

    LEFT_CANARY_ERROR = 32,
    RIGHT_CANARY_ERROR = 64

    #endif
};

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
    #ifdef _CANARY_PROTECTION

    canary_t* left_canary;

    #endif

    elem_t* data;

    #ifdef _CANARY_PROTECTION

    canary_t* right_canary;
    
    #endif

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
void stack_verify(stack* stk);

#ifdef _DEBUG

void stack_dump(stack* stk);

#endif

#endif