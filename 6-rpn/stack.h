#ifndef STACK_H
#define STACK_H

#include <stdlib.h>

#define STACK_MAX_SIZE 10

struct Stack
{
    int arr[STACK_MAX_SIZE];
    size_t count;
};

struct Stack stack_new();

void stack_push(struct Stack *stack, int value, int *err);

int stack_pop(struct Stack *stack);

int stack_peek(struct Stack stack);

void stack_print(struct Stack stack);

#endif