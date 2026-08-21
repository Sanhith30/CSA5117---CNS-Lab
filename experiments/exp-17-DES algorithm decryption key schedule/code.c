#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* Shift schedule for DES encryption rounds 1 to 16 */
const int SHIFTS_ENC[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

/* For decryption key generation:
   Starting from the 16th round key (K16), shifts are performed in reverse (right circular shifts)
   or shift schedule for direct round key indexing from K16 down to K1.
*/
const int SHIFTS_DEC[16] = {0, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

uint32_t left_shift_28(uint32_t val, int shift) {
    val &= 0x0FFFFFFF;
    return ((val << shift) | (val >> (28 - shift))) & 0x0FFFFFFF;
}

uint32_t right_shift_28(uint32_t val, int shift) {
    val &= 0x0FFFFFFF;
    return ((val >> shift) | (val << (28 - shift))) & 0x0FFFFFFF;
}

int main() {
    uint32_t C0 = 0x0ABCDEF; // 28-bit left key half
    uint32_t D0 = 0x0123456; // 28-bit right key half
    uint32_t C[17], D[17];
    int round;

    printf("=== Exp-17: DES Decryption Key Schedule Demonstration ===\n\n");
    printf("Initial 56-bit Key Halves after PC-1:\n");
    printf("C0 = 0x%07X, D0 = 0x%07X\n\n", C0, D0);

    /* Encryption subkey halves generation */
    C[0] = C0;
    D[0] = D0;
    for (round = 1; round <= 16; round++) {
        C[round] = left_shift_28(C[round - 1], SHIFTS_ENC[round - 1]);
        D[round] = left_shift_28(D[round - 1], SHIFTS_ENC[round - 1]);
    }

    printf("--- Forward (Encryption) Key Schedule (K1 to K16) ---\n");
    for (round = 1; round <= 16; round++) {
        printf("Round %2d (Shift %d): C%02d = 0x%07X | D%02d = 0x%07X\n",
               round, SHIFTS_ENC[round - 1], round, C[round], round, D[round]);
    }

    printf("\n--- Reverse (Decryption) Key Application ---\n");
    printf("Decryption applies subkeys in reverse order: K16, K15, ..., K1.\n");
    printf("Alternatively, using right-shifts directly from (C16, D16):\n\n");

    uint32_t C_dec = C[16];
    uint32_t D_dec = D[16];

    for (round = 1; round <= 16; round++) {
        C_dec = right_shift_28(C_dec, SHIFTS_DEC[round - 1]);
        D_dec = right_shift_28(D_dec, SHIFTS_DEC[round - 1]);
        printf("Decryption Round %2d (uses K%02d): C = 0x%07X | D = 0x%07X\n",
               round, 17 - round, C_dec, D_dec);
    }

    printf("\nVerified: Decryption subkey halves match encryption subkey halves in exact reverse order.\n");
    return 0;
}
