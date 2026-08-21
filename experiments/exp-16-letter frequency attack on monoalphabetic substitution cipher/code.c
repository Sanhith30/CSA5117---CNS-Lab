#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/* English letter frequency order from most to least frequent */
const char english_freq[] = "ETAOINSHRDLCUMWFGYPBVKJXQZ";

typedef struct {
    char letter;
    int count;
} LetterFreq;

int compare_freq(const void *a, const void *b) {
    return ((LetterFreq *)b)->count - ((LetterFreq *)a)->count;
}

int main() {
    char cipher[1000];
    LetterFreq freq[26];
    int i, cand, top_n, len;
    
    for (i = 0; i < 26; i++) {
        freq[i].letter = 'A' + i;
        freq[i].count = 0;
    }

    printf("=== Exp-16: Letter Frequency Attack on Monoalphabetic Substitution Cipher ===\n");
    printf("Enter Ciphertext: ");
    if (fgets(cipher, sizeof(cipher), stdin) == NULL) return 0;
    cipher[strcspn(cipher, "\r\n")] = '\0';

    len = strlen(cipher);
    for (i = 0; i < len; i++) {
        if (isalpha(cipher[i])) {
            freq[toupper(cipher[i]) - 'A'].count++;
        }
    }

    qsort(freq, 26, sizeof(LetterFreq), compare_freq);

    printf("\nCiphertext Letter Frequencies:\n");
    for (i = 0; i < 26; i++) {
        if (freq[i].count > 0)
            printf("%c: %d  ", freq[i].letter, freq[i].count);
    }
    printf("\n\nHow many top candidate plaintexts to display? (e.g., 10): ");
    if (scanf("%d", &top_n) != 1 || top_n <= 0) top_n = 10;
    if (top_n > 10) top_n = 10;

    printf("\n--- Top %d Candidate Plaintexts (Ranked by Heuristic Mapping) ---\n\n", top_n);

    for (cand = 0; cand < top_n; cand++) {
        char map[26];
        for (i = 0; i < 26; i++) map[i] = '?';

        for (i = 0; i < 26; i++) {
            int target_idx = (i + cand) % 26;
            map[freq[i].letter - 'A'] = english_freq[target_idx];
        }

        printf("Candidate #%2d: ", cand + 1);
        for (i = 0; i < len; i++) {
            if (isupper(cipher[i])) {
                printf("%c", map[cipher[i] - 'A']);
            } else if (islower(cipher[i])) {
                printf("%c", tolower(map[toupper(cipher[i]) - 'A']));
            } else {
                printf("%c", cipher[i]);
            }
        }
        printf("\n");
    }

    return 0;
}
