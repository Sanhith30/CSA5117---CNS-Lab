#include <stdio.h>
#include <string.h>
#include <ctype.h>

char matrix[5][5];

void generateMatrix(char key[])
{
    int used[26] = {0};
    int i, j, k = 0;
    char temp[100];

    strcpy(temp, key);

    for(i = 0; temp[i]; i++)
    {
        temp[i] = toupper(temp[i]);
        if(temp[i] == 'J')
            temp[i] = 'I';
    }

    used['J' - 'A'] = 1;

    for(i = 0; temp[i]; i++)
    {
        if(temp[i] >= 'A' && temp[i] <= 'Z')
        {
            if(!used[temp[i] - 'A'])
            {
                matrix[k / 5][k % 5] = temp[i];
                used[temp[i] - 'A'] = 1;
                k++;
            }
        }
    }

    for(i = 0; i < 26; i++)
    {
        if(!used[i])
        {
            matrix[k / 5][k % 5] = i + 'A';
            k++;
        }
    }
}

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
    char key[100];
    char cipher[200];
    int i;

    printf("Enter Keyword: ");
    scanf("%s", key);

    generateMatrix(key);

    printf("\nPlayfair Matrix:\n");

    for(i = 0; i < 5; i++)
    {
        int j;
        for(j = 0; j < 5; j++)
            printf("%c ", matrix[i][j]);
        printf("\n");
    }

    printf("\nEnter Cipher Text (without spaces): ");
    scanf("%s", cipher);

    printf("\nDecrypted Text: ");

    for(i = 0; cipher[i] != '\0'; i += 2)
    {
        int r1, c1, r2, c2;

        findPosition(toupper(cipher[i]), &r1, &c1);
        findPosition(toupper(cipher[i + 1]), &r2, &c2);

        if(r1 == r2)
        {
            printf("%c", matrix[r1][(c1 + 4) % 5]);
            printf("%c", matrix[r2][(c2 + 4) % 5]);
        }
        else if(c1 == c2)
        {
            printf("%c", matrix[(r1 + 4) % 5][c1]);
            printf("%c", matrix[(r2 + 4) % 5][c2]);
        }
        else
        {
            printf("%c", matrix[r1][c2]);
            printf("%c", matrix[r2][c1]);
        }
    }

    printf("\n");

    return 0;
}
