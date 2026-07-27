#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main()
{
    char text[100];
    int key, i;

    printf("Enter Plain Text: ");
    fgets(text, sizeof(text), stdin);

    printf("Enter Key (1-25): ");
    scanf("%d", &key);

    printf("\nEncrypted Text: ");

    for(i = 0; text[i] != '\0'; i++)
    {
        char ch = text[i];

        if(isupper(ch))
            printf("%c", ((ch - 'A' + key) % 26) + 'A');

        else if(islower(ch))
            printf("%c", ((ch - 'a' + key) % 26) + 'a');

        else
            printf("%c", ch);
    }

    return 0;
}
