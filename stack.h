#ifndef STACK_H
#define STACK_H

#include <cstdio>
#include <cstdint>

typedef int elem_t;

const int MIN_CAPACITY = 5;
const int GARBAGE = 696969;

#ifdef _CANARY_PROTECTION

typedef unsigned long long canary_t;
const canary_t CANARY_CONST = 0xDEADBABE;

#endif

const int ERRORS_NUM = 13;

enum stack_errors
{
    STACK_NULLPTR = 1,
    NEGATIVE_SIZE = 2,
    NEGATIVE_CAPACITY = 4,
    DATARRAY_NULLPTR  = 8,
    SIZE_BIGGER_THAN_CAPACITY = 16,
    TOO_BIG_MEMORYSIZE_FOR_CALLOC = 32,

    #ifdef _CANARY_PROTECTION
        LEFT_CANARY_DATA_ERROR    = 64,
        RIGHT_CANARY_DATA_ERROR   = 128,
        LEFT_CANARY_STRUCT_ERROR  = 256,
        RIGHT_CANARY_STRUCT_ERROR = 512,
    #endif

    #ifdef _HASH_PROTECTION
        HASH_DETECTED_INVALID_CHANGES_STRUCT = 1024,
        HASH_DETECTED_INVALID_CHANGES_DATA   = 2048,
        HASH_NOT_NULLIFIED                   = 4096,
    #endif
};  //do not forget to change errors num due to flags you have

#ifdef _DEBUG

struct function_info
{
    const char* variable_name;
    const char* function_name;
    const char* filename;
    int line;
};

#endif

struct stack
{
    #ifdef _CANARY_PROTECTION
        canary_t left_canary_struct;
    #endif

    #ifdef _HASH_PROTECTION
        long unsigned int hash_struct;
        long unsigned int hash_data;
    #endif

    elem_t* data;
    int capacity;
    int size;

    #ifdef _DEBUG
        function_info func_info;
    #endif

    #ifdef _CANARY_PROTECTION
        canary_t* left_canary_data;
        canary_t* right_canary_data;
    #endif

    #ifdef _CANARY_PROTECTION
        canary_t right_canary_struct;
    #endif
};

#ifdef _DEBUG
    #define ON_DEBUG(...)  __VA_ARGS__
#else
    #define ON_DEBUG(...)
#endif

#define STACK_CTOR(stk) stack_ctor(stk, ON_DEBUG( {#stk, __PRETTY_FUNCTION__, __FILE__, __LINE__} ))

void stack_ctor(stack* stk ON_DEBUG(, function_info func_info));

void stack_push(stack* stk, elem_t value);
void stack_pop(stack* stk);
void stack_dtor(stack* stk);

#ifdef _DEBUG
    void stack_dump(stack* stk);
    void stack_verify(stack* stk);
#endif

#ifdef _HASH_PROTECTION

long unsigned int poltorashka_hash(const char* key, long unsigned int len);

#define HASH_PROTECTION_FUNCTION_CALL() \
    stk->hash_struct = stk->hash_data = 0; \
    stk->hash_struct = poltorashka_hash((const char*) stk, sizeof(stack)); \
    stk->hash_data   = poltorashka_hash((const char*) stk->data, sizeof(elem_t) * stk->capacity); \

#endif

#endif
