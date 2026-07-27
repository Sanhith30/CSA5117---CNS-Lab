#include<stdio.h>
#include<string.h>
#include<ctype.h>

int inverse(int a)
{
    int i;

    for(i=1;i<26;i++)
        if((a*i)%26==1)
            return i;

    return -1;
}

int validA[]={
1,3,5,7,9,11,15,17,19,21,23,25
};

int main()
{
    char cipher[100];
    int i,j,k;

    printf("Enter Cipher Text: ");
    fgets(cipher,sizeof(cipher),stdin);

    for(i=0;i<12;i++)
    {
        int a=validA[i];
        int inv=inverse(a);

        for(j=0;j<26;j++)
        {
            printf("\na=%d b=%d : ",a,j);

            for(k=0;cipher[k]!='\0';k++)
            {
                char ch=cipher[k];

                if(isupper(ch))
                {
                    int x=(inv*((ch-'A')-j+26))%26;
                    printf("%c",x+'A');
                }
                else
                    printf("%c",ch);
            }
        }
    }

    return 0;
}
