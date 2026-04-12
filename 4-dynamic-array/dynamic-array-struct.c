#include <stdlib.h>
#include <stdio.h>

struct List
{
    int *array;
    size_t length;
    size_t capacity;
};

void list_new(struct List *list, int *err);
void list_append(struct List *list, int value, int *err);
void list_print(struct List list);
int list_get(struct List list, size_t index, int *err);
void list_free(struct List *list);

int main(void)
{
    int err = 0;
    struct List list;
    list_new(&list, &err);
    if (err != 0)
    {
        printf("error creating list\n");
        return err;
    }
    printf("created a new list: { length = %zu, capacity = %zu }\n", list.length, list.capacity);

    for (size_t i = 0; i < 1000000000; i++)
    {
        list_append(&list, i + 1, &err);
        if (err != 0)
        {
            printf("error appending to list\n");
            return err;
        }
    }

    printf("%d, %d, %d\n", list_get(list, 1, &err), list_get(list, 5, &err), list_get(list, 8, &err));
    if (err != 0)
    {
        printf("error reading value from list");
        return err;
    }

    list_print(list);
    list_free(&list);
}

void list_new(struct List *list, int *err)
{
    int initial_capacity = 4;

    list->capacity = initial_capacity;
    list->length = 0;
    if ((list->array = malloc(sizeof(*list->array) * list->capacity)) == NULL)
    {
        *err = 2;
        return;
    }
}

void list_append(struct List *list, int value, int *err)
{
    if (list->length == list->capacity)
    {
        int new_capacity = list->capacity * 2;
        if ((list->array = realloc(list->array, sizeof(*list->array) * new_capacity)) == NULL)
        {
            *err = 1;
            return;
        }
        list->capacity = new_capacity;
    }

    list->array[list->length] = value;
    list->length++;
}

void list_print(struct List list)
{
    printf("{\n");
    printf("  capacity = %zu\n", list.capacity);
    printf("  length = %zu\n", list.length);
    printf("  array = [");
    if (list.length <= 1000)
        for (size_t i = 0; i < list.length; i++)
        {
            printf("%d", list.array[i]);
            if (i < list.length - 1)
            {
                printf(", ");
            }
        }
    else
        printf("too large to print");
    printf("]\n");
    printf("}\n");
}

int list_get(struct List list, size_t index, int *err)
{
    if (index >= list.length)
    {
        *err = 1;
        return 0;
    }
    return list.array[index];
}

void list_free(struct List *list)
{
    free(list->array);
    list->array = NULL;
    list->length = 0;
    list->capacity = 0;
}