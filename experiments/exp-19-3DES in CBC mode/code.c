#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* Simplified 64-bit block cipher mock for 3DES (EDE: Encrypt K1, Decrypt K2, Encrypt K3) */
uint64_t mock_des(uint64_t block, uint64_t key, int encrypt) {
    uint64_t k = key ^ 0x5555555555555555ULL;
    if (encrypt) {
        return (block ^ k) + 0x0123456789ABCDEFULL;
    } else {
        return (block - 0x0123456789ABCDEFULL) ^ k;
    }
}

uint64_t triple_des_encrypt_block(uint64_t block, uint64_t k1, uint64_t k2, uint64_t k3) {
    uint64_t stage1 = mock_des(block, k1, 1);
    uint64_t stage2 = mock_des(stage1, k2, 0);
    uint64_t stage3 = mock_des(stage2, k3, 1);
    return stage3;
}

uint64_t triple_des_decrypt_block(uint64_t block, uint64_t k1, uint64_t k2, uint64_t k3) {
    uint64_t stage1 = mock_des(block, k3, 0);
    uint64_t stage2 = mock_des(stage1, k2, 1);
    uint64_t stage3 = mock_des(stage2, k1, 0);
    return stage3;
}

void print_hex64(uint64_t val) {
    printf("%08X%08X", (uint32_t)(val >> 32), (uint32_t)(val & 0xFFFFFFFF));
}

int main() {
    uint64_t K1 = 0x133457799BBCDFF1ULL;
    uint64_t K2 = 0x1122334455667788ULL;
    uint64_t K3 = 0x99AABBCCDDEEFF00ULL;
    uint64_t IV = 0xFEEDFACECAFEBEEFULL;

    uint64_t plaintext[3] = {
        0x48656C6C6F20576FULL, // "Hello Wo"
        0x726C642120334445ULL, // "rld! 3DE"
        0x5320434243202121ULL  // "S CBC !!"
    };
    uint64_t ciphertext[3];
    uint64_t decrypted[3];
    uint64_t prev;
    int i;

    printf("=== Exp-19: 3DES in Cipher Block Chaining (CBC) Mode ===\n\n");
    printf("Key 1: 0x"); print_hex64(K1); printf("\n");
    printf("Key 2: 0x"); print_hex64(K2); printf("\n");
    printf("Key 3: 0x"); print_hex64(K3); printf("\n");
    printf("IV   : 0x"); print_hex64(IV); printf("\n\n");

    /* CBC Encryption: C_i = 3DES_K(P_i XOR C_{i-1}) */
    prev = IV;
    printf("--- Encryption (CBC Mode) ---\n");
    for (i = 0; i < 3; i++) {
        uint64_t xor_in = plaintext[i] ^ prev;
        ciphertext[i] = triple_des_encrypt_block(xor_in, K1, K2, K3);
        prev = ciphertext[i];
        printf("Block %d: Plain = 0x", i + 1); print_hex64(plaintext[i]);
        printf(" -> Cipher = 0x"); print_hex64(ciphertext[i]); printf("\n");
    }

    /* CBC Decryption: P_i = 3DES_K_inv(C_i) XOR C_{i-1} */
    prev = IV;
    printf("\n--- Decryption (CBC Mode) ---\n");
    for (i = 0; i < 3; i++) {
        uint64_t dec_out = triple_des_decrypt_block(ciphertext[i], K1, K2, K3);
        decrypted[i] = dec_out ^ prev;
        prev = ciphertext[i];
        printf("Block %d: Cipher = 0x", i + 1); print_hex64(ciphertext[i]);
        printf(" -> Decrypted = 0x"); print_hex64(decrypted[i]); printf("\n");
    }

    printf("\n--- Analysis Questions ---\n");
    printf("a. For Security: AES (or 3-key 3DES over DES). AES provides 128/192/256-bit security against meet-in-the-middle and collision attacks.\n");
    printf("b. For Performance: AES is significantly faster than 3DES in both software and hardware due to single-pass SPN architecture.\n");

    return 0;
}
