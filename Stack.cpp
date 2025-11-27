#include "Stack.h"

int Stack_Ctor (stack_k* const Stack, const size_t Capacity)
{
    Stack->Array = (int*) calloc (Capacity + 2, sizeof (int));
    if (Stack->Array == NULL)
    {
        printf ("Error allocation memory for array stack\n");
        return There_Are_Errors_Stack;
    }

    Stack->Capacity = Capacity + 2;

    Stack->Array[0] = Canary_Stack;
    Stack->Array[Stack->Capacity - 1] = Canary_Stack;

    Stack->Size = 0;

    if (Stack_Error (Stack) != Not_Error_Stack)
    {
        Stack_Dump (Stack);
        printf ("Error in Stack_Ctor\n\n");
        return There_Are_Errors_Stack;
    }

    return Not_Error_Stack;
}

int Stack_Dtor (stack_k* const Stack)
{
    free (Stack->Array);
    Stack->Capacity = 0;
    Stack->Size = 0;

    return 0;
}

int Stack_Error (const stack_k* const Stack)
{
    if (Stack == NULL)
    {
        return Not_Struct_Stack;
    }

    if (Stack->Array == NULL)
    {
        return Not_Array_Stack;
    }

    if (Stack->Capacity == 0)
    {
        return Capacity_Null;
    }

    if (Stack->Size > Stack->Capacity)
    {
        return Bad_Size_Stack;
    }

    if (Stack->Array[0] != Canary_Stack || Stack->Array[Stack->Capacity - 1] != Canary_Stack)
    {
        return Bad_Canary;
    }

    return Not_Error_Stack;
}

int Stack_Dump (const stack_k* const Stack)
{
    int Stack_Err = Stack_Error (Stack);

    if (Stack_Err != 0)
    {
        printf ("\n\033[31m!!!EEERRROOORRR IN STACK!!!\033[0m\n");
    }

    if (Stack_Err == Not_Struct_Stack)
    {
        printf ("\nStruct with stack [%p] \033[31m(not struct stack!)\033[0m\n", Stack);
    }

    else
    {
        printf ("\nStruct with stack [%p]\n"
                "{\n", Stack);

        if (Stack_Err == Bad_Size_Stack)
        {
            printf ("    Size = %zu \033[31m(bad size!)\033[0m\n", Stack->Size);
        }

        else
        {
            printf ("    Size = %zu\n", Stack->Size);
        }

        if (Stack_Err == Capacity_Null)
        {
            printf ("    Capacity = %zu \033[31m(bad capacity!)\033[0m\n", Stack->Capacity);
        }

        else
        {
            printf ("    Capacity = %zu + 2\n", Stack->Capacity - 2);
        }

        if (Stack_Err == Not_Array_Stack)
        {
            printf ("    Stack [%p] \033[31m(not array stack!)\033[0m\n", Stack->Array);
        }

        else
        {
            printf ("    Stack [%p]\n"
                    "    {\n", Stack->Array);

            if (Stack->Array[0] != Canary_Stack)
            {
                printf ("       -[Start_Canary] = %d \033[31m(bad canary!)\033[0m\n", Stack->Array[0]);
            }

            else
            {
                printf ("       +[\033[32mStart_Canary\033[0m] = %d\n", Stack->Array[0]);
            }

            if (Stack_Err == Bad_Size_Stack)
            {
                for (size_t i = 0; i < Stack->Capacity - 2; i++)
                {
                    printf ("        [%zu] = %d\n", i, Stack->Array[i + 1]);
                }
            }

            else
            {
                for (size_t i = 0; i < Stack->Size; i++)
                {
                    printf ("       *[%zu] = %d\n", i, Stack->Array[i + 1]);
                }

                for (size_t i = Stack->Size; i < Stack->Capacity - 2; i++)
                {
                    printf ("        [%zu] = %d \033[31m(poison)\033[0m\n", i, Stack->Array[i + 1]);
                }
            }

            if (Stack->Array[Stack->Capacity - 1] != Canary_Stack)
            {
                printf ("       -[Finish_Canary] = %d \033[31m(bad canary!)\033[0m\n"
                        "    }\n", Stack->Array[Stack->Capacity - 1]);
            }

            else
            {
                printf ("       +[\033[32mFinish_Canary\033[0m] = %d\n"
                        "    }\n", Stack->Array[Stack->Capacity - 1]);
            }
        }

        printf ("}\n");
    }

    return 0;
}

int Stack_Reallocation (stack_k* const Stack)
{
    if (Stack->Capacity - 1 - 2 == Stack->Size)
    {
        Stack->Array[Stack->Capacity - 1] = 0;

        Stack->Capacity = Stack->Capacity * 2 - 2;

        Stack->Array = (int*) realloc (Stack->Array, Stack->Capacity * sizeof (int));
        if (Stack->Array == NULL)
        {
            printf ("Error allocation memory for array stack\n");
            return There_Are_Errors_Stack;
        }

        Stack->Array[Stack->Capacity - 1] = Canary_Stack;
    }

    return Not_Error_Stack;
}


int Stack_Get_Element (stack_k* const Stack, const size_t Number)
{
    if (Stack_Error (Stack) != Not_Error_Stack)
    {
        Stack_Dump (Stack);
        printf ("Error in start Stack_Push\n\n");
        return There_Are_Errors_Stack;
    }

    if (0 > Number || Number >= Stack->Size)
    {
        printf ("Bad Number\n\n");
        return There_Are_Errors_Stack;
    }

    return Stack->Array[Number + 1];
}

int Stack_Push        (stack_k* const Stack, const int Element)
{
    if (Stack_Error (Stack) != Not_Error_Stack)
    {
        Stack_Dump (Stack);
        printf ("Error in start Stack_Push\n\n");
        return There_Are_Errors_Stack;
    }

    if (Stack_Reallocation (Stack) == There_Are_Errors_Stack)
    {
        return There_Are_Errors_Stack;
    }

    Stack->Array[Stack->Size + 1] = Element;
    Stack->Size++;

    if (Stack_Error (Stack) != Not_Error_Stack)
    {
        Stack_Dump (Stack);
        printf ("Error in finish Stack_Push\n\n");
        return There_Are_Errors_Stack;
    }

    return Not_Error_Stack;
}

int Stack_Pop         (stack_k* const Stack)
{
    if (Stack_Error (Stack) != Not_Error_Stack)
    {
        Stack_Dump (Stack);
        printf ("Error in start Stack_Pop\n\n");
        return There_Are_Errors_Stack;
    }

    int Last_Element = Stack->Array[Stack->Size - 1 + 1];

    Stack->Array[Stack->Size - 1 + 1] = 0;
    Stack->Size--;

    if (Stack_Error (Stack) != Not_Error_Stack)
    {
        Stack_Dump (Stack);
        printf ("Error in finish Stack_Pop\n\n");
        return There_Are_Errors_Stack;
    }

    return Last_Element;
}

