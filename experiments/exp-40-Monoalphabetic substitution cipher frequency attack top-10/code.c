#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

const char english_frequencies[] = "ETAOINSHRDLCUMWFGYPBVKJXQZ";

typedef struct {
    char letter;
    int frequency;
} CharFreq;

int compare_frequency(const void *a, const void *b) {
    return ((CharFreq *)b)->frequency - ((CharFreq *)a)->frequency;
}

int main() {
    char ciphertext[1000];
    CharFreq freq[26];
    char mapping[26];
    int i, rank, top_n, len;

    for (i = 0; i < 26; i++) {
        freq[i].letter = 'A' + i;
        freq[i].frequency = 0;
    }

    printf("=== Exp-40: Automated Monoalphabetic Substitution Frequency Attack ===\n\n");
    printf("Enter Ciphertext: ");
    if (fgets(ciphertext, sizeof(ciphertext), stdin) == NULL) return 0;
    ciphertext[strcspn(ciphertext, "\r\n")] = '\0';

    len = strlen(ciphertext);
    for (i = 0; i < len; i++) {
        if (isalpha(ciphertext[i])) {
            freq[toupper(ciphertext[i]) - 'A'].frequency++;
        }
    }

    qsort(freq, 26, sizeof(CharFreq), compare_frequency);

    printf("\nSpecify number of top candidate plaintexts to display (e.g., 10): ");
    if (scanf("%d", &top_n) != 1 || top_n <= 0) top_n = 10;
    if (top_n > 26) top_n = 26;

    printf("\n--- Top %d Candidate Plaintexts (Ranked by Likelihood) ---\n\n", top_n);

    for (rank = 0; rank < top_n; rank++) {
        for (i = 0; i < 26; i++) {
            mapping[freq[i].letter - 'A'] = english_frequencies[(i + rank) % 26];
        }

        printf("Candidate #%2d: ", rank + 1);
        for (i = 0; i < len; i++) {
            if (isupper(ciphertext[i])) {
                printf("%c", mapping[ciphertext[i] - 'A']);
            } else if (islower(ciphertext[i])) {
                printf("%c", tolower(mapping[toupper(ciphertext[i]) - 'A']));
            } else {
                printf("%c", ciphertext[i]);
            }
        }
        printf("\n");
    }

    return 0;
}
