#include<stdio.h>
#include<string.h>
#include<ctype.h>

int gcd(int a,int b)
{
    while(b!=0)
    {
        int t=b;
        b=a%b;
        a=t;
    }
    return a;
}

int main()
{
    char text[100];
    int a,b,i;

    printf("Enter Plaintext: ");
    fgets(text,sizeof(text),stdin);

    printf("Enter value of a: ");
    scanf("%d",&a);

    printf("Enter value of b: ");
    scanf("%d",&b);

    if(gcd(a,26)!=1)
    {
        printf("Invalid value of a\n");
        return 0;
    }

    printf("Cipher Text: ");

    for(i=0;text[i]!='\0';i++)
    {
        char ch=text[i];

        if(isupper(ch))
            printf("%c",((a*(ch-'A')+b)%26)+'A');

        else if(islower(ch))
            printf("%c",((a*(ch-'a')+b)%26)+'a');

        else
            printf("%c",ch);
    }

    return 0;
}
