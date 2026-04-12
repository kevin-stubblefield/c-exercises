#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int my_atoi(char *s, int *err);

int main(void)
{
    char s[] = "     12-24";
    int err = 0;
    int parsed = my_atoi(s, &err);
    if (err != 0)
        printf("error: cannot convert string to number - %d\n", err);
    else
        printf("parsed: %d\n", parsed);
}

int my_atoi(char *s, int *err)
{
    int is_negative = 0;
    int has_encountered_number = 0;
    int result = 0;

    for (size_t i = 0; i < strlen(s); i++)
    {
        if (s[i] == '-' && !is_negative && !has_encountered_number)
        {
            is_negative = 1;
            continue;
        }
        else if (s[i] == '-' && (is_negative || has_encountered_number))
        {
            *err = 2;
            return 0;
        }

        if (s[i] == ' ' ||
            s[i] == '\t' ||
            s[i] == '\n')
        {
            if (has_encountered_number)
            {
                *err = 3;
                return 0;
            }
            continue;
        }

        int curr = s[i] - '0';
        if (curr >= 0 && curr <= 9)
        {
            result = result * 10 + curr;
            has_encountered_number = 1;
        }
        else
        {
            *err = 1;
            return 0;
        }
    }

    if (is_negative)
    {
        result *= -1;
    }

    return result;
}