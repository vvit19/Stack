#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "stack.h"
#include "stack_errors.h"

static elem_t* stack_recalloc(elem_t** array, int new_size, int old_size);

#ifdef _DEBUG

void stack_ctor(stack* stk, const char* variable_name, const char* function_name,
                const char* filename, int line)
{
    assert(stk);

    stk->data = (elem_t*) calloc(MIN_CAPACITY, sizeof(elem_t));
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
        stk->data = stack_recalloc(&stk->data, stk->capacity, stk->capacity - MIN_CAPACITY);
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
        stk->data = (elem_t*) realloc(stk->data, stk->capacity * sizeof(elem_t));
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
}

static elem_t* stack_recalloc(elem_t** array, int new_size, int old_size)
{
    *array = (elem_t*) realloc(*array, new_size * sizeof(elem_t));

    for (int i = old_size; i < new_size; i++)
    {
        (*array)[i] = GARBAGE;
    }

    return *array;
}