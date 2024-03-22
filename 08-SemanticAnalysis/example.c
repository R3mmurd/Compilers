#include <stdio.h>

// int(int)
int accum_from_zero_to(int x)
{
    int i;
    int total = 0;

    for (i = 0; i < x; i++)
    {
        total = total + i;
    }

    return total;
}

int main()
{
    int result = accum_from_zero_to(5);
    printf("%d\n", result);
    return 0;
}