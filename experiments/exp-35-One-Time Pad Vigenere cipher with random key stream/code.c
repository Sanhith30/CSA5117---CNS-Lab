#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

int main() {
    char plaintext[200];
    int key[200];
    char ciphertext[200];
    char decrypted[200];
    int i, len;

    srand(time(NULL));

    printf("=== Exp-35: One-Time Pad (OTP) Vigenere Cipher with Random Key Stream ===\n\n");
    printf("Enter Plaintext: ");
    if (fgets(plaintext, sizeof(plaintext), stdin) == NULL) return 0;
    plaintext[strcspn(plaintext, "\r\n")] = '\0';

    len = strlen(plaintext);

    /* Generate truly random key shifts between 0 and 25 for each character */
    printf("\nRandom Key Stream Generated (shifts 0..25):\n");
    for (i = 0; i < len; i++) {
        key[i] = rand() % 26;
        printf("%2d ", key[i]);
    }
    printf("\n\n");

    /* Encryption: C_i = (P_i + K_i) mod 26 */
    for (i = 0; i < len; i++) {
        if (isupper(plaintext[i])) {
            ciphertext[i] = ((plaintext[i] - 'A' + key[i]) % 26) + 'A';
        } else if (islower(plaintext[i])) {
            ciphertext[i] = ((plaintext[i] - 'a' + key[i]) % 26) + 'a';
        } else {
            ciphertext[i] = plaintext[i];
        }
    }
    ciphertext[len] = '\0';

    /* Decryption: P_i = (C_i - K_i + 26) mod 26 */
    for (i = 0; i < len; i++) {
        if (isupper(ciphertext[i])) {
            decrypted[i] = ((ciphertext[i] - 'A' - key[i] + 26) % 26) + 'A';
        } else if (islower(ciphertext[i])) {
            decrypted[i] = ((ciphertext[i] - 'a' - key[i] + 26) % 26) + 'a';
        } else {
            decrypted[i] = ciphertext[i];
        }
    }
    decrypted[len] = '\0';

    printf("Original Plaintext  : %s\n", plaintext);
    printf("Encrypted Ciphertext: %s\n", ciphertext);
    printf("Decrypted Plaintext : %s\n\n", decrypted);

    printf("Verification: %s\n", (strcmp(plaintext, decrypted) == 0) ? "SUCCESS (Perfect match)" : "FAILED");
    return 0;
}
