#include <stdlib.h>
#include <stdio.h>

int da_new(int **da, size_t capacity);
int da_push(int **da, int value, int *length, int *capacity);
int da_get(int *da, size_t index, int length);
void da_free(int *da);
void da_print(int *da, int length);

int main(void)
{
    int capacity = 4;
    int *da = NULL;
    int err;
    err = da_new(&da, capacity);
    if (err == -1)
    {
        return 1;
    }
    int length = 0;

    for (int i = 0; i < 10; i++)
    {
        err = da_push(&da, i + 1, &length, &capacity);
        if (err == -1)
        {
            return 2;
        }
    }

    da_print(da, length);
    printf("%d, %d, %d\n", da_get(da, 3, length), da_get(da, 6, length), da_get(da, 7, length));
    da_free(da);
}

void da_print(int *da, int length)
{
    printf("[");
    for (int i = 0; i < length; i++)
    {
        printf("%d", da[i]);
        if (i < length - 1)
        {
            printf(", ");
        }
    }
    printf("]\n");
}

int da_new(int **da, size_t capacity)
{
    *da = malloc(sizeof(**da) * capacity);
    if (*da == NULL)
    {
        printf("Failed to allocate a new array with capacity %zu\n", capacity);
        return -1;
    }
    return 0;
}

int da_push(int **da, int value, int *length, int *capacity)
{
    if (*length == *capacity)
    {
        int new_capacity = *capacity * 2;
        if ((*da = realloc(*da, sizeof(**da) * new_capacity)) == NULL)
        {
            printf("Failed to reallocate array");
            return -1;
        }
        *capacity = new_capacity;
    }

    (*da)[*length] = value;
    *length += 1;

    return 0;
}

int da_get(int *da, size_t index, int length)
{
    if ((int)index >= length)
    {
        printf("index out of range");
        return -1;
    }
    return da[index];
}

void da_free(int *da)
{
    free(da);
}