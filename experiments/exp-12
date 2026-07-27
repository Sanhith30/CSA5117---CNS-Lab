#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main()
{
    int key[2][2] = {{9,4},{5,7}};
    char text[100], plain[100];
    int i, j = 0;

    printf("Enter Plain Text: ");
    fgets(text, sizeof(text), stdin);

    // Remove spaces and convert to uppercase
    for(i = 0; text[i] != '\0'; i++)
    {
        if(isalpha(text[i]))
        {
            plain[j++] = toupper(text[i]);
        }
    }
    plain[j] = '\0';

    // Padding if odd length
    if(j % 2 != 0)
    {
        plain[j] = 'X';
        plain[j + 1] = '\0';
        j++;
    }

    printf("\nCipher Text: ");

    for(i = 0; i < j; i += 2)
    {
        int p1 = plain[i] - 'A';
        int p2 = plain[i + 1] - 'A';

        int c1 = (key[0][0] * p1 + key[0][1] * p2) % 26;
        int c2 = (key[1][0] * p1 + key[1][1] * p2) % 26;

        printf("%c%c", c1 + 'A', c2 + 'A');
    }

    return 0;
}
