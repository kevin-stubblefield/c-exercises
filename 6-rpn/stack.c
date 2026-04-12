#include <stdio.h>
#include "stack.h"

struct Stack stack_new()
{
    struct Stack s = {.arr = {0}, .count = 0};
    return s;
}

void stack_push(struct Stack *stack, int value, int *err)
{
    if (stack->count < STACK_MAX_SIZE)
    {
        stack->arr[stack->count++] = value;
    }
    else
    {
        *err = 1;
        printf("stack overflow\n");
    }
}

int stack_pop(struct Stack *stack)
{
    if (stack->count > 0)
    {
        int value = stack->arr[stack->count - 1];
        stack->arr[--stack->count] = 0;
        return value;
    }
    return 0;
}

int stack_peek(struct Stack stack)
{
    if (stack.count > 0)
    {
        return stack.arr[stack.count - 1];
    }
    return 0;
}

void stack_print(struct Stack stack)
{
    for (int i = (int)stack.count - 1; i >= 0; i--)
    {
        printf("| %d |\n", stack.arr[i]);
    }
    printf("|___|\n");
}