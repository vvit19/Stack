#include <cstdio>
#include <cstdlib>

#include "stack.h"
#include "stack_errors.h"

static int find_stack_errors(stack* stk);
static void fprint_error(int error_value);

void stack_verify(stack* stk)
{
    int errors = find_stack_errors(stk);
    int error_value = 1;

    for (int num = 0; num < ERRORS_NUM; num++)
    {
        if (num > 0) error_value *= 2;

        if (errors & error_value) fprint_error(error_value);
    }
}

static int find_stack_errors(stack* stk)
{
    int errors = 0;

    if (stk == nullptr)       errors |= STACK_NULLPTR;
    if (stk->size < 0)        errors |= NEGATIVE_SIZE;
    if (stk->capacity < 0)    errors |= NEGATIVE_CAPACITY;
    if (stk->data == nullptr) errors |= DATARRAY_NULLPTR;

    return errors;
}

static void fprint_error(int error_value)
{ 
    FILE* log_file = fopen(LOG_FILE, "a");
    if (log_file == nullptr)
    {
        fprintf(log_file, "LOG_FILE NOT FOUND\n");
        exit(1);
    }

    switch (error_value)
    {
    case STACK_NULLPTR:
        fprintf(log_file, "STACK_NULLPTR ERROR\n");
        exit(1);
        //break;
    case NEGATIVE_SIZE:
        fprintf(log_file, "NEGATIVE_SIZE ERROR\n");
        exit(1);
        //break;
    case NEGATIVE_CAPACITY:
        fprintf(log_file, "NEGATIVE_CAPACITY ERROR\n");
        exit(1);
        //break;
    case DATARRAY_NULLPTR:
        fprintf(log_file, "DATA ARRAY NULLPTR\n");
        exit(1);
        //break;
    default:
        fprintf(log_file, "Ooooh...we don't know what error was happened\n");
        break;
    }

    fclose(log_file);
}

void stack_dump(stack* stk, const char* variable_name, const char* function_name, 
                const char* program_name, int line)
{
    FILE* log_file = fopen(LOG_FILE, "a");
    if (log_file == nullptr)
    {
        fprintf(log_file, "LOG_FILE NOT FOUND\n");
        exit(1);
    }

    fprintf(log_file, DUMP_STRUCTURE, &stk, variable_name, program_name, line, function_name,
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