#include <stdio.h>
#include <string.h>
#include "linked-list.h"

void print_int(void *);
void print_float(void *);
void print_string(void *);
void *multiply_by_2(void *data);
int greater_than_4(void *data);

int main(void)
{
    int err = 0;
    struct SinglyLinkedList list;
    sll_new(&list);
    for (int i = 0; i < 10; i++)
    {
        sll_append(&list, &i, sizeof(int), &err);
        if (err != 0)
        {
            printf("error occurred in sll_append\n");
            return 1;
        }
    }

    // struct SinglyLinkedList new_list = sll_map(list, sizeof(int), &err, multiply_by_2);
    struct SinglyLinkedList new_list = sll_filter(list, sizeof(int), &err, greater_than_4);
    sll_print(new_list, print_int);
    return 0;
}

void print_int(void *data)
{
    printf("%d", *(int *)data);
}

void print_float(void *data)
{
    printf("%.1f", *(float *)data);
}

void print_string(void *data)
{
    printf("%s", (char *)data);
}

void *multiply_by_2(void *data)
{
    int result = *(int *)data * 2;
    void *p_result = malloc(sizeof(int));
    p_result = memcpy(p_result, &result, sizeof(int));
    return p_result;
}

int greater_than_4(void *data)
{
    return *(int *)data > 4;
}