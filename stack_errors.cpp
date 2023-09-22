#include <cstdio>
#include <cstdlib>

#include "stack.h"

static int  find_stack_errors(stack* stk);
static void tell_error(int error_value);

void stack_verify(stack* stk)
{
    int errors = find_stack_errors(stk);
    int error_value = 1;

    for (int num = 0; num < ERRORS_NUM; num++)
    {
        if (num > 0) 
        {
            error_value *= 2;
        }

        if (errors & error_value) 
        {
            #ifdef _DEBUG

            stack_dump(stk);
            tell_error(error_value);

            #else

            abort();

            #endif
        }
    }
}

static int find_stack_errors(stack* stk)
{
    int errors = 0;

    if (stk == nullptr)            errors |= STACK_NULLPTR;
    if (stk->size < 0)             errors |= NEGATIVE_SIZE;
    if (stk->capacity < 0)         errors |= NEGATIVE_CAPACITY;
    if (stk->data == nullptr)      errors |= DATARRAY_NULLPTR;
    if (stk->size > stk->capacity) errors |= SIZE_BIGGER_THAN_CAPACITY;
    
    #ifdef _CANARY_PROTECTION

    if (*stk->left_canary  != CANARY_CONST) errors |=  LEFT_CANARY_ERROR;
    if (*stk->right_canary != CANARY_CONST) errors |= RIGHT_CANARY_ERROR;

    #endif

    return errors;
}

#ifdef _DEBUG

#define DUMP_STRUCTURE \
    "stack[%X] %s called from %s(%d) %s()\n" \
    "size = %d\n" \
    "capacity = %d\n" \
    "data[%X]\n\n" \
    
#define LOG_FILE "log.txt"

void stack_dump(stack* stk)
{
    static bool file_was_cleaned = false;

    FILE* log_file = nullptr;

    if (file_was_cleaned)
    {
        log_file = fopen(LOG_FILE, "a");
    } 
    else 
    {
        log_file = fopen(LOG_FILE, "w");
        file_was_cleaned = true;
    }

    if (log_file == nullptr)
    {
        fprintf(log_file, "LOG_FILE NOT FOUND\n");
        abort();
    }

    fprintf(log_file, DUMP_STRUCTURE, (int) stk, 
            stk->func_call_info.variable_name, stk->func_call_info.filename, 
            stk->func_call_info.line, stk->func_call_info.function_name,
            stk->size, stk->capacity, (int) stk->data);

    for (int i = 0; i < stk->capacity; i++)
    {
        if (stk->data[i] == GARBAGE)
        {
            fprintf(log_file, "*[%d] = NAN (POISON)\n", i);
            continue;
        }
        
        fprintf(log_file, "*[%d] = %d\n", i, stk->data[i]);
    }

    fprintf(log_file, "--------------------------------------------------------------------------------------\n\n");
    fclose(log_file);
}

static void tell_error(int error_value)
{ 
    FILE* log_file = fopen(LOG_FILE, "a");
    if (log_file == nullptr)
    {
        fprintf(log_file, "LOG_FILE NOT FOUND\n");
        abort();
    }

    switch (error_value)
    {
    case STACK_NULLPTR:
        fprintf(log_file, "STACK_NULLPTR ERROR\n");
        abort();
    case NEGATIVE_SIZE:
        fprintf(log_file, "NEGATIVE_SIZE ERROR\n");
        abort();
    case NEGATIVE_CAPACITY:
        fprintf(log_file, "NEGATIVE_CAPACITY ERROR\n");
        abort();
    case DATARRAY_NULLPTR:
        fprintf(log_file, "DATA ARRAY NULLPTR\n");
        abort();
    case SIZE_BIGGER_THAN_CAPACITY:
        fprintf(log_file, "SIZE IS BIGGER THAN CAPACITY\n");
        abort();
    
    #ifdef _CANARY_PROTECTION

    case LEFT_CANARY_ERROR:
        fprintf(log_file, "LEFT CANARY ERROR. SOMEONE WAS TRYING TO CHANGE DATA VALUES (not with push or pop)");
        abort();
    case RIGHT_CANARY_ERROR:
        fprintf(log_file, "RIGHT CANARY ERROR. SOMEONE WAS TRYING TO CHANGE DATA VALUES (not with push or pop)");
        abort();

    #endif

    default:
        fprintf(log_file, "Ooooh...we don't know what error was happened\n");
        break;
    }

    fclose(log_file);
}

#endif
