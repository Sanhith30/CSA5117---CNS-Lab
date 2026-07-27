#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main()
{
    char text[100];
    int key[100];
    int i, len;

    printf("Enter Plain Text (without spaces): ");
    scanf("%s", text);

    len = strlen(text);

    printf("Enter %d Key Values:\n", len);

    for(i = 0; i < len; i++)
        scanf("%d", &key[i]);

    printf("\nCipher Text: ");

    for(i = 0; i < len; i++)
    {
        char ch = toupper(text[i]);

        if(ch >= 'A' && ch <= 'Z')
        {
            int p = ch - 'A';
            int c = (p + key[i]) % 26;

            printf("%c", c + 'A');
        }
    }

    return 0;
}
