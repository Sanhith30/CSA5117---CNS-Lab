#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main()
{
    char cipher[100];
    int key, i;

    printf("Enter Cipher Text: ");
    scanf("%s", cipher);

    printf("\nPossible Plain Texts:\n\n");

    for(key = 0; key < 26; key++)
    {
        printf("Key %2d : ", key);

        for(i = 0; cipher[i] != '\0'; i++)
        {
            char ch = cipher[i];

            if(isupper(ch))
                printf("%c", ((ch - 'A' - key + 26) % 26) + 'A');

            else if(islower(ch))
                printf("%c", ((ch - 'a' - key + 26) % 26) + 'a');

            else
                printf("%c", ch);
        }

        printf("\n");
    }

    return 0;
}
