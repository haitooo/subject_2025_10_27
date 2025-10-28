#include <stdio.h>

int gcd(int a, int b)
{
    int t;

    if (a < 0)
        a = -a;
    if (b < 0)
        b = -b;
    while (b != 0)
    {
        t = a % b;
        a = b;
        b = t;
    }
    return (a);
}

int main(void)
{
    int x;
    int g = 0;
    int first = 1;

    printf("Input natural numbers:");
    while (scanf("%d", &x) == 1)
    {
        if (x <= 0)
        {
            break ;
        }
        if (first)
        {
            g = x;
            first = 0;
        }
        else
        {
            g = gcd(g, x);
        }
    }
    printf("\ngcd = %d\n", g);
    return (0);
}
