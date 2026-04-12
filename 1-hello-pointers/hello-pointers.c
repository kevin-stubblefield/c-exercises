#include <stdio.h>

void swap(int *a, int *b);

int main(void)
{
    int a = 8;
    int b = 12;

    printf("before swap: a: %d, b: %d\n", a, b);
    swap(&a, &b);
    printf("after swap: a: %d, b: %d\n", a, b);
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}