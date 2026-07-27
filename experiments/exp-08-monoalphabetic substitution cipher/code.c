#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main()
{
    char plain[100];
    char cipher[] = "CIPHERABDFGJKLMNOQSTUVWXYZ";
    int i;

    printf("Enter Plain Text: ");
    fgets(plain, sizeof(plain), stdin);

    printf("Cipher Text: ");

    for(i=0; plain[i]!='\0'; i++)
    {
        if(isupper(plain[i]))
            printf("%c", cipher[plain[i]-'A']);
        else if(islower(plain[i]))
            printf("%c", tolower(cipher[plain[i]-'a']));
        else
            printf("%c", plain[i]);
    }

    return 0;
}
