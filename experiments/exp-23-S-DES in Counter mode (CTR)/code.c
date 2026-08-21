#include <stdio.h>
#include <stdint.h>

/* S-DES Tables */
const int P10[10] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
const int P8[8]   = {6, 3, 7, 4, 8, 5, 10, 9};
const int IP[8]   = {2, 6, 3, 1, 4, 8, 5, 7};
const int IP_INV[8] = {4, 1, 3, 5, 7, 2, 8, 6};
const int EP[8]   = {4, 1, 2, 3, 2, 3, 4, 1};
const int P4[4]   = {2, 4, 3, 1};

const int S0[4][4] = {
    {1, 0, 3, 2},
    {3, 2, 1, 0},
    {0, 2, 1, 3},
    {3, 1, 3, 2}
};

const int S1[4][4] = {
    {0, 1, 2, 3},
    {2, 0, 1, 3},
    {3, 0, 1, 0},
    {2, 1, 0, 3}
};

uint16_t permute(uint16_t in, const int *table, int n, int in_len) {
    uint16_t out = 0;
    int i;
    for (i = 0; i < n; i++) {
        int bit = (in >> (in_len - table[i])) & 1;
        out = (out << 1) | bit;
    }
    return out;
}

void sdes_key_gen(uint16_t key10, uint8_t *k1, uint8_t *k2) {
    uint16_t p10 = permute(key10, P10, 10, 10);
    uint8_t left = (p10 >> 5) & 0x1F;
    uint8_t right = p10 & 0x1F;

    left = ((left << 1) | (left >> 4)) & 0x1F;
    right = ((right << 1) | (right >> 4)) & 0x1F;
    *k1 = permute((left << 5) | right, P8, 8, 10);

    left = ((left << 2) | (left >> 3)) & 0x1F;
    right = ((right << 2) | (right >> 3)) & 0x1F;
    *k2 = permute((left << 5) | right, P8, 8, 10);
}

uint8_t fk(uint8_t nibble, uint8_t sk) {
    uint8_t ep = permute(nibble, EP, 8, 4);
    uint8_t l, r, s_out;
    int r0, c0, r1, c1;

    ep ^= sk;
    l = (ep >> 4) & 0xF;
    r = ep & 0xF;

    r0 = ((l & 8) >> 2) | (l & 1);
    c0 = (l >> 1) & 3;
    r1 = ((r & 8) >> 2) | (r & 1);
    c1 = (r >> 1) & 3;

    s_out = (S0[r0][c0] << 2) | S1[r1][c1];
    return permute(s_out, P4, 4, 4);
}

uint8_t sdes_encrypt(uint8_t block, uint8_t k1, uint8_t k2) {
    uint8_t ip = permute(block, IP, 8, 8);
    uint8_t l = (ip >> 4) & 0xF;
    uint8_t r = ip & 0xF;

    uint8_t f1 = fk(r, k1);
    uint8_t sw = (r << 4) | (l ^ f1);

    uint8_t f2, pre_inv;
    l = (sw >> 4) & 0xF;
    r = sw & 0xF;
    f2 = fk(r, k2);
    pre_inv = ((l ^ f2) << 4) | r;

    return permute(pre_inv, IP_INV, 8, 8);
}

void print_bin8(uint8_t val) {
    int i;
    for (i = 7; i >= 0; i--) printf("%d", (val >> i) & 1);
}

int main() {
    uint16_t key = 0x1FD; // 10-bit key: 01111 11101
    uint8_t counter = 0x00;
    uint8_t P[3] = {0x01, 0x02, 0x04};
    uint8_t C[3], D[3];
    uint8_t k1, k2;
    int i;

    printf("=== Exp-23: S-DES in Counter (CTR) Mode ===\n\n");
    sdes_key_gen(key, &k1, &k2);

    printf("Key    : 01111 11101\n");
    printf("Initial Counter: "); print_bin8(counter); printf("\n\n");

    /* CTR Mode Encryption: C_i = P_i XOR S-DES(Counter + i) */
    printf("--- CTR Encryption ---\n");
    for (i = 0; i < 3; i++) {
        uint8_t cur_ctr = counter + i;
        uint8_t keystream = sdes_encrypt(cur_ctr, k1, k2);
        C[i] = P[i] ^ keystream;

        printf("Block %d: Plain = ", i + 1); print_bin8(P[i]);
        printf(" | CTR = "); print_bin8(cur_ctr);
        printf(" | Keystream = "); print_bin8(keystream);
        printf(" | Cipher = "); print_bin8(C[i]); printf("\n");
    }

    printf("\nGenerated Ciphertext: ");
    print_bin8(C[0]); printf(" "); print_bin8(C[1]); printf(" "); print_bin8(C[2]); printf("\n");

    /* CTR Mode Decryption: P_i = C_i XOR S-DES(Counter + i) */
    printf("\n--- CTR Decryption ---\n");
    for (i = 0; i < 3; i++) {
        uint8_t cur_ctr = counter + i;
        uint8_t keystream = sdes_encrypt(cur_ctr, k1, k2);
        D[i] = C[i] ^ keystream;

        printf("Block %d: Cipher = ", i + 1); print_bin8(C[i]);
        printf(" -> Decrypted = "); print_bin8(D[i]); printf("\n");
    }

    return 0;
}
