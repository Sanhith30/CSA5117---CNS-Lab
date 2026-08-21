#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* NIST SP 800-38B CMAC Subkey Generation Constants:
   - For 64-bit blocks (e.g., 3DES):  Rb = 0x000000000000001B (x^64 + x^4 + x^3 + x + 1)
   - For 128-bit blocks (e.g., AES):  Rb = 0x87 (x^128 + x^7 + x^2 + x + 1)
*/

#define CONST_64  0x000000000000001BULL
#define CONST_128 0x87

/* Derives K1 and K2 for 64-bit block size */
void cmac_subkeys_64(uint64_t L, uint64_t *K1, uint64_t *K2) {
    if ((L & (1ULL << 63)) == 0) {
        *K1 = (L << 1);
    } else {
        *K1 = (L << 1) ^ CONST_64;
    }

    if ((*K1 & (1ULL << 63)) == 0) {
        *K2 = (*K1 << 1);
    } else {
        *K2 = (*K1 << 1) ^ CONST_64;
    }
}

/* Derives K1 and K2 for 128-bit block size (represented as two 64-bit words) */
typedef struct {
    uint64_t hi;
    uint64_t lo;
} uint128_t;

uint128_t left_shift_128(uint128_t val) {
    uint128_t res;
    res.hi = (val.hi << 1) | (val.lo >> 63);
    res.lo = (val.lo << 1);
    return res;
}

void cmac_subkeys_128(uint128_t L, uint128_t *K1, uint128_t *K2) {
    int msb_L = (L.hi >> 63) & 1;
    int msb_K1;
    *K1 = left_shift_128(L);
    if (msb_L) K1->lo ^= CONST_128;

    msb_K1 = (K1->hi >> 63) & 1;
    *K2 = left_shift_128(*K1);
    if (msb_K1) K2->lo ^= CONST_128;
}

void print_hex64(uint64_t val) {
    printf("%08X%08X", (uint32_t)(val >> 32), (uint32_t)(val & 0xFFFFFFFF));
}

int main() {
    uint64_t L_64 = 0x8A8E9A34B16F3D4AULL;
    uint64_t K1_64, K2_64;
    uint128_t L_128;
    uint128_t K1_128, K2_128;

    L_128.hi = 0x7DF76B0C1AB899B3ULL;
    L_128.lo = 0x3E42F047B91B546FULL;

    printf("=== Exp-31: CMAC Subkey Generation Algorithm ===\n\n");

    /* 64-bit demonstration */
    cmac_subkeys_64(L_64, &K1_64, &K2_64);

    printf("[1. 64-bit Block Size (Rb = 0x1B)]\n");
    printf("L  = E_K(0^64) : 0x"); print_hex64(L_64); printf("\n");
    printf("Subkey 1 (K1)  : 0x"); print_hex64(K1_64); printf("\n");
    printf("Subkey 2 (K2)  : 0x"); print_hex64(K2_64); printf("\n\n");

    /* 128-bit demonstration */
    cmac_subkeys_128(L_128, &K1_128, &K2_128);

    printf("[2. 128-bit Block Size (Rb = 0x87)]\n");
    printf("L  = E_K(0^128): 0x"); print_hex64(L_128.hi); print_hex64(L_128.lo); printf("\n");
    printf("Subkey 1 (K1)  : 0x"); print_hex64(K1_128.hi); print_hex64(K1_128.lo); printf("\n");
    printf("Subkey 2 (K2)  : 0x"); print_hex64(K2_128.hi); print_hex64(K2_128.lo); printf("\n\n");

    printf("--- Questions & Answers ---\n");
    printf("a. What constants are needed for block sizes of 64 and 128 bits?\n");
    printf("   - For 64-bit  block: Rb = 0x1B (binary: 0001 1011)\n");
    printf("   - For 128-bit block: Rb = 0x87 (binary: 1000 0111)\n\n");
    printf("b. How the left shift and XOR accomplishes the desired result:\n");
    printf("   - Left shift by 1 corresponds to polynomial multiplication by x in the Galois Field GF(2^n).\n");
    printf("   - If the MSB is 1 (overflow outside GF field), conditional XOR with irreducible polynomial Rb\n");
    printf("     performs modular reduction modulo P(x), ensuring result stays within field GF(2^n).\n");

    return 0;
}
