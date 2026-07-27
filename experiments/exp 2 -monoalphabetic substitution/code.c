#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main()
{
    char plain[100];
    char key[] = "QWERTYUIOPASDFGHJKLZXCVBNM";
    int i;

    printf("Enter Plain Text: ");
    fgets(plain, sizeof(plain), stdin);

    printf("\nEncrypted Text: ");

    for(i = 0; plain[i] != '\0'; i++)
    {
        char ch = plain[i];

        if(isupper(ch))
            printf("%c", key[ch-'A']);

        else if(islower(ch))
            printf("%c", tolower(key[ch-'a']));

        else
            printf("%c", ch);
    }

    return 0;
}
