#include <stdio.h>
#include <string.h>
#include <ctype.h>

char matrix[5][5];

void generateMatrix()
{
    char key[] = "MONARCHY";
    char alphabet[] = "ABCDEFGHIKLMNOPQRSTUVWXYZ";
    int used[26] = {0};
    int i, j, k = 0;

    used['J'-'A'] = 1;

    for(i=0; key[i]; i++)
    {
        char ch = toupper(key[i]);
        if(!used[ch-'A'])
        {
            matrix[k/5][k%5] = ch;
            used[ch-'A'] = 1;
            k++;
        }
    }

    for(i=0; alphabet[i]; i++)
    {
        char ch = alphabet[i];
        if(!used[ch-'A'])
        {
            matrix[k/5][k%5] = ch;
            used[ch-'A'] = 1;
            k++;
        }
    }
}

void findPosition(char ch,int *row,int *col)
{
    if(ch=='J')
        ch='I';

    int i,j;

    for(i=0;i<5;i++)
        for(j=0;j<5;j++)
            if(matrix[i][j]==ch)
            {
                *row=i;
                *col=j;
            }
}

int main()
{
    char text[100];
    int i;

    generateMatrix();

    printf("Playfair Matrix:\n");
    for(i=0;i<5;i++)
    {
        for(int j=0;j<5;j++)
            printf("%c ",matrix[i][j]);
        printf("\n");
    }

    printf("\nEnter Plain Text (Even letters): ");
    scanf("%s",text);

    printf("Encrypted Text: ");

    for(i=0;text[i];i+=2)
    {
        int r1,c1,r2,c2;

        findPosition(toupper(text[i]),&r1,&c1);
        findPosition(toupper(text[i+1]),&r2,&c2);

        if(r1==r2)
        {
            printf("%c%c",
            matrix[r1][(c1+1)%5],
            matrix[r2][(c2+1)%5]);
        }
        else if(c1==c2)
        {
            printf("%c%c",
            matrix[(r1+1)%5][c1],
            matrix[(r2+1)%5][c2]);
        }
        else
        {
            printf("%c%c",
            matrix[r1][c2],
            matrix[r2][c1]);
        }
    }

    return 0;
}
