#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "stack.h"

static elem_t* stack_recalloc(stack* stk, int new_size, int old_size);

#ifdef _DEBUG

void stack_ctor(stack* stk, const char* variable_name, const char* function_name,
                const char* filename, int line)
{
    assert(stk);

    #ifdef _CANARY_PROTECTION

    char* temp = (char*) calloc(1, MIN_CAPACITY * sizeof(elem_t) + 2 * sizeof(canary_t));

    stk->left_canary  = (canary_t*) temp;
    stk->right_canary = (canary_t*) ((size_t) temp + MIN_CAPACITY * sizeof(elem_t) + sizeof(canary_t));

    stk->data = (elem_t*) ((size_t) temp + sizeof(canary_t));

    *(stk->left_canary) = CANARY_CONST;
    *(stk->right_canary) = CANARY_CONST;

    #else    

    stk->data = (elem_t*) calloc(MIN_CAPACITY, sizeof(elem_t));

    #endif

    stk->size = 0;
    stk->capacity = MIN_CAPACITY;

    stk->func_call_info.filename = filename;
    stk->func_call_info.function_name = function_name;
    stk->func_call_info.variable_name = variable_name;
    stk->func_call_info.line = line;

    stack_verify(stk);
}

#else

void stack_ctor(stack* stk)
{
    assert(stk);

    stk->data = (elem_t*) calloc(MIN_CAPACITY, sizeof(elem_t));
    stk->size = 0;
    stk->capacity = MIN_CAPACITY;

    stack_verify(stk);
}

#endif

void stack_push(stack* stk, elem_t value)
{
    stack_verify(stk);

    if (stk->size >= stk->capacity)
    {
        stk->capacity += MIN_CAPACITY;

        stk->data = stack_recalloc(stk, stk->capacity, stk->capacity - MIN_CAPACITY);
    }

    stk->data[stk->size++] = value;

    stack_verify(stk);
}

void stack_pop(stack* stk)
{
    stack_verify(stk);

    if (stk->size <= stk->capacity / 4 && stk->capacity >= MIN_CAPACITY)
    {
        stk->capacity /= 2;
        stk->data = (elem_t*) stack_recalloc(stk, stk->capacity, stk->capacity * 2);
    }    
  
    stk->size--;
    stk->data[stk->size] = GARBAGE;

    stack_verify(stk);
}

void stack_dtor(stack* stk)
{
    stack_verify(stk);

    stk->data = nullptr;
    free(stk->data);

    stk->capacity = 0;
    stk->size = 0;

    #ifdef _CANARY_PROTECTION

    stk->left_canary = nullptr;
    free(stk->left_canary);
 
    stk->right_canary = nullptr;
    free(stk->right_canary);

    #endif
}

static elem_t* stack_recalloc(stack* stk, int new_size, int old_size)
{
    #ifdef _CANARY_PROTECTION

    *(stk->left_canary) = *(stk->right_canary) = GARBAGE;

    char* temp = (char*)  ((size_t) stk->data - sizeof(canary_t));

    temp = (char*) realloc(temp, new_size * sizeof(elem_t) + 2 * sizeof(canary_t));

    stk->data = (elem_t*) (temp + sizeof(canary_t));

    stk->left_canary  = (canary_t*) temp;
    stk->right_canary = (canary_t*) ((size_t) temp + stk->capacity * sizeof(elem_t) + sizeof(canary_t));

    stk->data = (elem_t*) ((size_t) temp + sizeof(canary_t));

    *(stk->left_canary) = *(stk->right_canary) = CANARY_CONST;  

    #else

    stk->data = (elem_t*) realloc(stk->data, new_size * sizeof(elem_t));

    #endif

    if (new_size > old_size)
    {
        for (int i = old_size; i < new_size; i++)
        {
            (stk->data)[i] = GARBAGE;
        }
    }

    return stk->data;
}