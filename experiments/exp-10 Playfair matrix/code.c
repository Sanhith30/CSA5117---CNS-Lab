#include <stdio.h>
#include <string.h>
#include <ctype.h>

char matrix[5][5] = {
    {'M','F','H','I','K'},
    {'U','N','O','P','Q'},
    {'Z','V','W','X','Y'},
    {'E','L','A','R','G'},
    {'D','S','T','B','C'}
};

void findPosition(char ch, int *row, int *col)
{
    int i, j;

    if(ch == 'J')
        ch = 'I';

    for(i = 0; i < 5; i++)
    {
        for(j = 0; j < 5; j++)
        {
            if(matrix[i][j] == ch)
            {
                *row = i;
                *col = j;
                return;
            }
        }
    }
}

int main()
{
    char text[100];
    char plain[100];
    int i, j = 0;

    printf("Enter Plain Text (without spaces): ");
    scanf("%s", text);

    // Convert to uppercase and replace J with I
    for(i = 0; text[i] != '\0'; i++)
    {
        plain[j] = toupper(text[i]);

        if(plain[j] == 'J')
            plain[j] = 'I';

        j++;
    }
    plain[j] = '\0';

    printf("\nCipher Text: ");

    for(i = 0; i < strlen(plain); i += 2)
    {
        int r1, c1, r2, c2;

        findPosition(plain[i], &r1, &c1);
        findPosition(plain[i + 1], &r2, &c2);

        if(r1 == r2)
        {
            printf("%c%c",
                   matrix[r1][(c1 + 1) % 5],
                   matrix[r2][(c2 + 1) % 5]);
        }
        else if(c1 == c2)
        {
            printf("%c%c",
                   matrix[(r1 + 1) % 5][c1],
                   matrix[(r2 + 1) % 5][c2]);
        }
        else
        {
            printf("%c%c",
                   matrix[r1][c2],
                   matrix[r2][c1]);
        }
    }

    printf("\n");

    return 0;
}
