#include <stdio.h>
#include <stdint.h>

uint32_t simple_encrypt(uint32_t p, uint32_t k) { return (p ^ k) + 0x12345678; }
uint32_t simple_decrypt(uint32_t c, uint32_t k) { return (c - 0x12345678) ^ k; }

int main() {
    uint32_t P[4] = {0x11111111, 0x22222222, 0x33333333, 0x44444444};
    uint32_t K = 0xA5A5A5A5;
    uint32_t IV = 0x12345678;
    uint32_t C_ecb[4], C_cbc[4];
    uint32_t P_ecb_rec[4], P_cbc_rec[4];
    int i;

    printf("=== Exp-20: Error Propagation in ECB vs CBC Modes ===\n\n");

    /* Normal Encryption */
    uint32_t prev = IV;
    for (i = 0; i < 4; i++) {
        C_ecb[i] = simple_encrypt(P[i], K);
        C_cbc[i] = simple_encrypt(P[i] ^ prev, K);
        prev = C_cbc[i];
    }

    /* Simulate 1-bit Transmission Error in C[0] (C1) */
    printf("--- Simulating 1-bit Transmission Error in Transmitted Ciphertext C1 ---\n");
    uint32_t C_ecb_corrupt[4], C_cbc_corrupt[4];
    for (i = 0; i < 4; i++) {
        C_ecb_corrupt[i] = C_ecb[i];
        C_cbc_corrupt[i] = C_cbc[i];
    }
    C_ecb_corrupt[0] ^= 0x00000001; // Corrupt C1
    C_cbc_corrupt[0] ^= 0x00000001; // Corrupt C1

    /* Decrypt ECB */
    printf("\n[ECB Mode with Corrupted C1]:\n");
    for (i = 0; i < 4; i++) {
        P_ecb_rec[i] = simple_decrypt(C_ecb_corrupt[i], K);
        printf("P%d: Expected = 0x%08X, Received = 0x%08X -> %s\n",
               i + 1, P[i], P_ecb_rec[i], (P[i] == P_ecb_rec[i]) ? "CORRECT" : "CORRUPTED");
    }

    /* Decrypt CBC */
    printf("\n[CBC Mode with Corrupted C1]:\n");
    prev = IV;
    for (i = 0; i < 4; i++) {
        P_cbc_rec[i] = simple_decrypt(C_cbc_corrupt[i], K) ^ prev;
        prev = C_cbc_corrupt[i];
        printf("P%d: Expected = 0x%08X, Received = 0x%08X -> %s\n",
               i + 1, P[i], P_cbc_rec[i], (P[i] == P_cbc_rec[i]) ? "CORRECT" : "CORRUPTED");
    }

    printf("\n--- Analysis & Answers ---\n");
    printf("a. Are any blocks beyond P2 affected when C1 is corrupted in transmission?\n");
    printf("   Answer: NO. In CBC decryption (Pi = D(Ci) XOR Ci-1), C1 only affects P1 (via D(C1)) and P2 (via XOR with C1).\n");
    printf("   P3, P4, etc., decrypt completely normally.\n\n");
    printf("b. If there is a bit error in the source version of P1 during CBC encryption:\n");
    printf("   Answer: The error in P1 corrupts C1. That corrupted C1 feeds forward into C2, C3, ..., Cn.\n");
    printf("   Therefore, ALL subsequent ciphertext blocks C1..Cn are corrupted during transmission.\n");
    printf("   At the receiver, P1 is received with the original bit error, but P2..Pn are successfully recovered!\n");

    return 0;
}
