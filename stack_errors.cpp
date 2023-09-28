#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>

#include "stack.h"

static long long int find_stack_errors(stack* stk);
static void tell_error(long long int error_value);

void stack_verify(stack* stk)
{
    long long int errors = find_stack_errors(stk);
    long long int error_value = 1;

    for (int num = 0; num < ERRORS_NUM; num++)
    {
        if (num > 0)
        {
            error_value *= 2;
        }

        if (errors & error_value)
        {
            #ifdef _DEBUG

            tell_error(error_value);
            stack_dump(stk);

            #else

            exit(1);

            #endif
        }
    }
}

static long long int find_stack_errors(stack* stk)
{
    long long int errors = 0;

    if (stk == nullptr)
    {
        errors |= STACK_NULLPTR;
        tell_error(STACK_NULLPTR);

        return errors;
    }
    if (stk->size == DTOR_GARBAGE)
    {
        errors |= TWICE_DTORED;
        tell_error(TWICE_DTORED);

        return errors;
    }
    if (stk->data == nullptr)
    {
        errors |= DATARRAY_NULLPTR;
        tell_error(DATARRAY_NULLPTR);

        return errors;
    }
    if (stk->size < 0)
    {
        errors |= NEGATIVE_SIZE;
    }
    if (stk->capacity < 0)
    {
        errors |= NEGATIVE_CAPACITY;
    }
    if (stk->size > stk->capacity)
    {
        errors |= SIZE_BIGGER_THAN_CAPACITY;
    }
    if (stk->capacity * sizeof(elem_t) >= SIZE_MAX)
    {
        errors |= TOO_BIG_MEMORYSIZE_FOR_CALLOC;
    }

    #ifdef _CANARY_PROTECTION

    if (memcmp(stk->left_canary_data,  &CANARY_CONST, sizeof(canary_t)) != 0) errors |=  LEFT_CANARY_DATA_ERROR;
    if (memcmp(stk->right_canary_data, &CANARY_CONST, sizeof(canary_t)) != 0) errors |= RIGHT_CANARY_DATA_ERROR;

    #endif

    #ifdef _HASH_PROTECTION

    long unsigned int hash_struct_ref = stk->hash_struct;
    long unsigned int hash_data_ref   = stk->hash_data;

    stk->hash_struct = stk->hash_data = 0;
    stk->hash_struct = poltorashka_hash((const char*) stk, sizeof(stack));

    if (stk->hash_struct != hash_struct_ref)
    {
        errors |= HASH_DETECTED_INVALID_CHANGES_STRUCT;
        tell_error(HASH_DETECTED_INVALID_CHANGES_STRUCT);

        return errors;
    }

    stk->hash_data = poltorashka_hash((const char*) stk->data, sizeof(elem_t) * stk->capacity);

    if (stk->hash_data != hash_data_ref)
    {
        errors |= HASH_DETECTED_INVALID_CHANGES_DATA;
    }

    #endif

    return errors;
}

#ifdef _DEBUG

#define DUMP_STRUCTURE \
    "stack[0x%p] %s called from %s(%d) %s\n" \
    "size = %lld\n" \
    "capacity = %lld\n" \
    "data[0x%p]\n\n" \

#define LOG_FILE "log.txt"

void stack_dump(stack* stk)
{
    FILE* log_file = nullptr;
    log_file = fopen(LOG_FILE, "a");

    if (log_file == nullptr)
    {
        fprintf(log_file, "LOG_FILE NOT FOUND\n");
        exit(1);
    }

    fprintf(log_file, DUMP_STRUCTURE, stk,
            stk->func_info.variable_name, stk->func_info.filename,
            stk->func_info.line, stk->func_info.function_name,
            stk->size, stk->capacity, stk->data);

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

static void tell_error(long long int error_value)
{
    FILE* log_file = fopen(LOG_FILE, "a");
    if (log_file == nullptr)
    {
        fprintf(log_file, "LOG_FILE NOT FOUND\n\n");
        exit(1);
    }

    switch (error_value)
    {
    case STACK_NULLPTR:
        fprintf(log_file, "STACK_NULLPTR ERROR\n\n");
        exit(1);
    case NEGATIVE_SIZE:
        fprintf(log_file, "NEGATIVE_SIZE ERROR\n\n");
        exit(1);
    case NEGATIVE_CAPACITY:
        fprintf(log_file, "NEGATIVE_CAPACITY ERROR\n\n");
        exit(1);
    case DATARRAY_NULLPTR:
        fprintf(log_file, "DATA ARRAY NULLPTR\n\n");
        exit(1);
    case SIZE_BIGGER_THAN_CAPACITY:
        fprintf(log_file, "SIZE IS BIGGER THAN CAPACITY\n\n");
        exit(1);
    case TOO_BIG_MEMORYSIZE_FOR_CALLOC:
        fprintf(log_file, "TOO BIG MEMORY SIZE FOR CALLOC (SO CAN'T ALLOCATE)\n\n");
        exit(1);
    case TWICE_DTORED:
        fprintf(log_file, "STACK DTORED TWICE\n\n");
        exit(1);

    #ifdef _CANARY_PROTECTION

    case LEFT_CANARY_STRUCT_ERROR:
        fprintf(log_file, "LEFT CANARY ERROR. SOMEONE WAS TRYING TO CHANGE STRUCT VALUES (not with push or pop)\n\n");
        exit(1);
    case RIGHT_CANARY_STRUCT_ERROR:
        fprintf(log_file, "RIGHT CANARY ERROR. SOMEONE WAS TRYING TO CHANGE STRUCT VALUES (not with push or pop)\n\n");
        exit(1);
    case LEFT_CANARY_DATA_ERROR:
        fprintf(log_file, "LEFT CANARY ERROR. SOMEONE WAS TRYING TO CHANGE DATA VALUES (not with push or pop)\n\n");
        //exit(1);
        break;
    case RIGHT_CANARY_DATA_ERROR:
        fprintf(log_file, "RIGHT CANARY ERROR. SOMEONE WAS TRYING TO CHANGE DATA VALUES (not with push or pop)\n\n");
        //exit(1);
        break;

    #endif

    #ifdef _HASH_PROTECTION

    case HASH_DETECTED_INVALID_CHANGES_STRUCT:
        fprintf(log_file, "HASH DETECTED INVALID CHANGES. SOMEONE WAS TRYING TO CHANGE STRUCT VALUES\n\n");
        exit(1);
    case HASH_DETECTED_INVALID_CHANGES_DATA:
        fprintf(log_file, "HASH DETECTED INVALID CHANGES. SOMEONE WAS TRYING TO CHANGE DATA VALUES\n\n");
        //exit(1);
        break;
    #endif

    default:
        fprintf(log_file, "Ooooh...we don't know what error was happened\n\n");
        break;
    }

    fclose(log_file);
}
#endif
