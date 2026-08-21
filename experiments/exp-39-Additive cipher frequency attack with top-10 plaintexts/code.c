#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/* Expected standard English letter frequencies (%) */
const double eng_letter_freq[26] = {
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966, 0.153,
    0.772, 4.025, 2.406, 6.749,  7.507, 1.929, 0.095, 5.987, 6.327, 9.056,
    2.758, 0.978, 2.360, 0.150,  1.974, 0.074
};

typedef struct {
    int key;
    double score;
    char text[1000];
} Candidate;

int cmp_candidates(const void *a, const void *b) {
    double diff = ((Candidate *)a)->score - ((Candidate *)b)->score;
    if (diff < 0) return -1;
    if (diff > 0) return 1;
    return 0;
}

int main() {
    char cipher[1000];
    Candidate cand[26];
    int i, key, len, top_n;

    printf("=== Exp-39: Automated Frequency Attack on Additive (Caesar) Cipher ===\n\n");
    printf("Enter Ciphertext: ");
    if (fgets(cipher, sizeof(cipher), stdin) == NULL) return 0;
    cipher[strcspn(cipher, "\r\n")] = '\0';

    len = strlen(cipher);

    /* Evaluate all 26 potential keys */
    for (key = 0; key < 26; key++) {
        cand[key].key = key;
        int counts[26] = {0};
        int total_alpha = 0;

        for (i = 0; i < len; i++) {
            if (isalpha(cipher[i])) {
                char base = isupper(cipher[i]) ? 'A' : 'a';
                char p = ((cipher[i] - base - key + 26) % 26) + base;
                cand[key].text[i] = p;
                counts[toupper(p) - 'A']++;
                total_alpha++;
            } else {
                cand[key].text[i] = cipher[i];
            }
        }
        cand[key].text[len] = '\0';

        /* Compute Chi-Square statistic against English frequencies */
        double chi_sq = 0.0;
        for (i = 0; i < 26; i++) {
            double expected = (eng_letter_freq[i] / 100.0) * total_alpha;
            if (expected > 0) {
                double diff = counts[i] - expected;
                chi_sq += (diff * diff) / expected;
            }
        }
        cand[key].score = chi_sq;
    }

    qsort(cand, 26, sizeof(Candidate), cmp_candidates);

    printf("\nSpecify number of top candidate plaintexts to display (e.g., 10): ");
    if (scanf("%d", &top_n) != 1 || top_n <= 0) top_n = 10;
    if (top_n > 26) top_n = 26;

    printf("\n--- Top %d Candidate Plaintexts (Ranked by Minimum Chi-Square Score) ---\n\n", top_n);
    for (i = 0; i < top_n; i++) {
        printf("Rank #%2d (Key %2d | Chi-Sq Score: %7.2f): %s\n",
               i + 1, cand[i].key, cand[i].score, cand[i].text);
    }

    return 0;
}
