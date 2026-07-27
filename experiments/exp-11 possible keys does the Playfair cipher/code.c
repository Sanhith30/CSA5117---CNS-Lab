#include <stdio.h>

int main()
{
    long double fact = 1;
    int i;

    for(i = 1; i <= 25; i++)
    {
        fact = fact * i;
    }

    printf("Number of possible Playfair keys = %.0Lf\n", fact);
    printf("Approximate power of 2 = 2^84\n");

    return 0;
}
