#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "hash-table.h"

void str_tolower(char *);

int main(void)
{
    FILE *file = fopen("test-files/word-frequency.txt", "r");
    char line[1024];
    ht *table = ht_create();

    while (fgets(line, sizeof(line), file))
    {
        str_tolower(line);
        char *token = strtok(line, " ,.\n");
        while (token != NULL && token[0] != '\n')
        {
            void *value = ht_get(table, token);
            if (value == NULL)
            {
                int *initial = malloc(sizeof(int));
                *initial = 1;
                const char *key = ht_set(table, token, (void *)initial);
                if (key == NULL)
                {
                    printf("error: failed to set token: %s\n", token);
                    return 1;
                }
            }
            else
            {
                int *count = (int *)value;
                *count += 1;
            }
            token = strtok(NULL, " ,.\n");
        }
    }

    fclose(file);

    hti iterator = ht_iterator(table);
    while (ht_next(&iterator))
    {
        printf("%s: %d\n", iterator.key, *(int *)iterator.value);
        free(iterator.value);
    }

    hti new_iterator = ht_iterator(table);
    while (ht_next(&new_iterator))
    {
        printf("%s\n", "I pee my pants");
        free(new_iterator.value);
    }

    ht_destroy(table);
    return 0;
}

void str_tolower(char *value)
{
    for (int i = 0; value[i]; i++)
    {
        value[i] = tolower(value[i]);
    }
}