#include <stdio.h>
#include <string.h>

char palindrome(char *str)
{
    int j = strlen(str) - 1;
    if(j < 1)
        return 'n';
    for(int i = 0; i < j; i++)
    {
        if(str[i] != str[j])
            return 'n';
        j--;
    }
    return 'y';
}

int main(int ac, char **av)
{
    if(ac != 2)
        return 1;
    printf("%c\n", palindrome(av[1]));

}