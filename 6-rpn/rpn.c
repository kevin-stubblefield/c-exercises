#include "stack.h"
#include <stdio.h>
#include <string.h>

int parse_rpn(char *input, int *err);

int main(int argc, char **argv)
{
    int err = 0;
    char input[50];
    for (int i = 1; i < argc; i++)
    {
        int value = parse_rpn(argv[i], &err);
        if (err > 0)
        {
            printf("%s -> failed\n", argv[i]);
            continue;
        }
        printf("%s -> %d\n", argv[i], value);
    }

    if (argc == 1)
    {
        printf("enter a string reverse polish notation: ");
        fgets(input, sizeof(input), stdin);

        input[strcspn(input, "\n")] = '\0';

        int value = parse_rpn(input, &err);
        if (err > 0)
        {
            printf("%s -> failed\n", input);
            return err;
        }
        printf("%s -> %d\n", input, value);
    }
    return 0;
}

int parse_rpn(char *input, int *err)
{
    int j = 0;
    *err = 0;

    struct Stack stack = stack_new();
    for (int i = 0; input[i] != '\0'; i = j)
    {
        char curr[20] = "";
        int k = 0;
        for (j = i; input[j] != '\0'; j++)
        {
            if (input[j] == ' ')
            {
                j++;
                break;
            }
            curr[k++] = input[j];
        }

        char *endptr;
        int num = (int)strtol(curr, &endptr, 10);

        if (*endptr == '\0')
        {
            stack_push(&stack, num, err);
            if (*err > 0)
            {
                return 0;
            }
        }
        else
        {
            if (strlen(endptr) != 1)
            {
                *err = 2;
                printf("non-numerical value is too long to be an operator\n");
                return 0;
            }

            if (stack.count == 1)
            {
                *err = 3;
                printf("supported operators are unable to operate on one operand\n");
                return 0;
            }

            int operand2 = stack_pop(&stack);
            int operand1 = stack_pop(&stack);
            switch (curr[0])
            {
            case '+':
                stack_push(&stack, operand1 + operand2, err);
                break;
            case '-':
                stack_push(&stack, operand1 - operand2, err);
                break;
            case '*':
                stack_push(&stack, operand1 * operand2, err);
                break;
            case '/':
                if (operand2 == 0)
                {
                    *err = 4;
                    printf("divide by 0 IS ILLEGAL GUY\n");
                    return 0;
                }
                stack_push(&stack, operand1 / operand2, err);
                break;
            default:
                *err = 1;
                printf("unknown operand. abort\n");
                return 0;
            }
        }
    }
    int calculation = stack_pop(&stack);
    return calculation;
}