#include <stdio.h>
#include <stdint.h>

/* DES Permuted Choice 2 (PC-2) table (1-indexed positions from 56-bit CD) */
const int PC2[48] = {
    14, 17, 11, 24,  1,  5,
     3, 28, 15,  6, 21, 10,
    23, 19, 12,  4, 26,  8,
    16,  7, 27, 20, 13,  2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

int main() {
    int i;
    int c_count = 0, d_count = 0;
    int c_error = 0, d_error = 0;

    printf("=== Exp-18: Analysis of DES PC-2 Subkey Disjoint 28-bit Halves ===\n\n");

    printf("First 24 bits of PC-2 (derived from Left 28-bit half C, positions 1-28):\n");
    for (i = 0; i < 24; i++) {
        printf("%2d ", PC2[i]);
        if ((i + 1) % 6 == 0) printf("\n");
        if (PC2[i] >= 1 && PC2[i] <= 28) c_count++;
        else c_error++;
    }

    printf("\nSecond 24 bits of PC-2 (derived from Right 28-bit half D, positions 29-56):\n");
    for (i = 24; i < 48; i++) {
        printf("%2d ", PC2[i]);
        if ((i + 1 - 24) % 6 == 0) printf("\n");
        if (PC2[i] >= 29 && PC2[i] <= 56) d_count++;
        else d_error++;
    }

    printf("\n--- Verification Results ---\n");
    printf("1. Bits in first 24-bit half from C [1-28]  : %d/24 (Errors: %d)\n", c_count, c_error);
    printf("2. Bits in second 24-bit half from D [29-56]: %d/24 (Errors: %d)\n", d_count, d_error);
    
    if (c_error == 0 && d_error == 0) {
        printf("\nCONCLUSION: Verified! The first 24 bits of each 48-bit subkey come exclusively\n");
        printf("from the left 28-bit subset (C), and the remaining 24 bits come exclusively from\n");
        printf("the disjoint right 28-bit subset (D).\n");
    }

    return 0;
}
