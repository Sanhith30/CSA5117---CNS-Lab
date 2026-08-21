#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define BLOCK_SIZE 8

/* Demonstrates unambiguous padding rule: always append 0x80 then pad with 0x00 */
int pad_message(const uint8_t *in, int len, uint8_t *out) {
    int padded_len;
    memcpy(out, in, len);
    out[len] = 0x80; // 1-bit followed by 7 zeros
    padded_len = len + 1;
    while (padded_len % BLOCK_SIZE != 0) {
        out[padded_len++] = 0x00;
    }
    return padded_len;
}

int unpad_message(const uint8_t *in, int padded_len, uint8_t *out) {
    int i = padded_len - 1;
    while (i >= 0 && in[i] == 0x00) i--;
    if (i >= 0 && in[i] == 0x80) {
        memcpy(out, in, i);
        out[i] = '\0';
        return i;
    }
    return -1; // Padding corrupted
}

int main() {
    uint8_t full_block_msg[] = "NETWORK8"; // Exactly 8 bytes (1 block)
    uint8_t padded[32], recovered[32];
    int p_len, u_len, i;

    printf("=== Exp-34: Motivation for Mandatory Padding Block in Block Ciphers ===\n\n");
    printf("Original Message: \"%s\" (Length: 8 bytes = Exact multiple of 8)\n\n", full_block_msg);

    p_len = pad_message(full_block_msg, 8, padded);

    printf("Padded Message (Total %d bytes / %d blocks):\n", p_len, p_len / BLOCK_SIZE);
    for (i = 0; i < p_len; i++) {
        printf("%02X ", padded[i]);
        if ((i + 1) % BLOCK_SIZE == 0) printf(" | ");
    }
    printf("\n\n");

    u_len = unpad_message(padded, p_len, recovered);
    printf("Unpadded Output: \"%s\" (Length: %d bytes)\n\n", recovered, u_len);

    printf("--- MOTIVATION / EXPLANATION ---\n");
    printf("Why pad when message length is already an exact multiple of the block size?\n\n");
    printf("1. Ambiguity Removal: Without mandatory padding, if a payload ends naturally with bytes\n");
    printf("   like 0x80 0x00, a receiver cannot determine if they are authentic data or padding.\n");
    printf("2. Reversibility: Making padding unconditional guarantees that the receiver ALWAYS strips\n");
    printf("   the trailing 0x80 0x00... pattern from the final block with 100%% certainty.\n");

    return 0;
}
