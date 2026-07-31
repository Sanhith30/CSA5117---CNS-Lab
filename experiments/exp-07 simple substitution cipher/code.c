#include <stdio.h>

int main()
{
    char cipher[] = "53@@#305))6*;4826)4@.)4@);806*;48#8$60))85;;]8*;:@*8#83";
    int i;
    char ch;

    printf("Decrypted Text:\n");

    for(i = 0; cipher[i] != '\0'; i++)
    {
        ch = cipher[i];

        switch(ch)
        {
            case '5': printf("A"); break;
            case '3': printf("G"); break;
            case '@': printf("O"); break;
            case '#': printf("D"); break;
            case '0': printf("L"); break;
            case ')': printf("E"); break;
            case '6': printf("S"); break;
            case '*': printf("N"); break;
            case ';': printf("T"); break;
            case '4': printf("H"); break;
            case '8': printf("R"); break;
            case '2': printf("I"); break;
            case '.': printf("F"); break;
            case '$': printf("B"); break;
            case ']': printf("Y"); break;
            case ':': printf("P"); break;
            case '(': printf("M"); break;
            case '9': printf("C"); break;
            case '?': printf("U"); break;
            case '1': printf("V"); break;
            case '&': printf("W"); break;
            default: printf("%c", ch);
        }
    }

    printf("\n");

    return 0;
}
