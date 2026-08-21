#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

const char eng_freq[] = "ETAOINSHRDLCUMWFGYPBVKJXQZ";

typedef struct {
    char letter;
    int count;
} FreqPair;

int cmp_freq(const void *a, const void *b) {
    return ((FreqPair *)b)->count - ((FreqPair *)a)->count;
}

int main() {
    char ciphertext[1000];
    FreqPair freq[26];
    char map[26];
    int i, k, top_n, len;

    for (i = 0; i < 26; i++) {
        freq[i].letter = 'A' + i;
        freq[i].count = 0;
    }

    printf("=== Exp-37: Automated Monoalphabetic Substitution Frequency Attack ===\n\n");
    printf("Enter Ciphertext: ");
    if (fgets(ciphertext, sizeof(ciphertext), stdin) == NULL) return 0;
    ciphertext[strcspn(ciphertext, "\r\n")] = '\0';

    len = strlen(ciphertext);
    for (i = 0; i < len; i++) {
        if (isalpha(ciphertext[i])) {
            freq[toupper(ciphertext[i]) - 'A'].count++;
        }
    }

    qsort(freq, 26, sizeof(FreqPair), cmp_freq);

    printf("\nSpecify number of top candidate plaintexts to generate (e.g. 10): ");
    if (scanf("%d", &top_n) != 1 || top_n <= 0) top_n = 10;
    if (top_n > 26) top_n = 26;

    printf("\n--- Top %d Candidate Plaintexts (Ranked by Likelihood) ---\n\n", top_n);
    for (k = 0; k < top_n; k++) {
        for (i = 0; i < 26; i++) {
            map[freq[i].letter - 'A'] = eng_freq[(i + k) % 26];
        }

        printf("Rank #%2d: ", k + 1);
        for (i = 0; i < len; i++) {
            if (isupper(ciphertext[i])) {
                printf("%c", map[ciphertext[i] - 'A']);
            } else if (islower(ciphertext[i])) {
                printf("%c", tolower(map[toupper(ciphertext[i]) - 'A']));
            } else {
                printf("%c", ciphertext[i]);
            }
        }
        printf("\n");
    }

    return 0;
}
