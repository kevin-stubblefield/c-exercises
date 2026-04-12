#include <stdio.h>

int string_length(char *str);
int my_strlen(char *str);
int strlen_p(char *str);

int main(void)
{
    // initializes a char array
    char str[] = "Hello World!";
    // initializes a string literal (readonly), stored far away from program's memory
    // char *str = "Hello World!";

    printf("before: %s\n", str);
    string_reverse(str);
    printf("after: %s\n", str);
}

int my_strlen(char *str)
{
    int count = 0;

    while (str[count] != '\0')
    {
        count++;
    }

    return count;
}

int strlen_p(char *s)
{
    char *p = s;

    while (*p != '\0')
    {
        p++;
    }

    return p - s;
}

void string_reverse(char *str)
{
    int len = strlen_p(str);
    for (int i = 0; i < len / 2; i++)
    {
        char temp = *(str + i);
        char other = *(str + len - i - 1);
        str[i] = other;
        str[len - i - 1] = temp;
    }
}