#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define BLOCK_SIZE 8 // 8 bytes (64 bits)

/* Applies 10* padding (0x80 followed by 0x00 bytes) */
int apply_padding(const uint8_t *input, int len, uint8_t *output) {
    int padded_len;
    memcpy(output, input, len);
    
    // Always append 0x80 (1-bit followed by 7 zeros)
    output[len] = 0x80;
    
    padded_len = len + 1;
    while (padded_len % BLOCK_SIZE != 0) {
        output[padded_len++] = 0x00;
    }
    return padded_len;
}

/* Strips 10* padding */
int remove_padding(const uint8_t *input, int padded_len, uint8_t *output) {
    int i = padded_len - 1;
    while (i >= 0 && input[i] == 0x00) {
        i--;
    }
    if (i >= 0 && input[i] == 0x80) {
        int original_len = i;
        memcpy(output, input, original_len);
        output[original_len] = '\0';
        return original_len;
    }
    return -1; // Padding error
}

void print_hex(const char *label, const uint8_t *data, int len) {
    int i;
    printf("%s (%d bytes): ", label, len);
    for (i = 0; i < len; i++) printf("%02X ", data[i]);
    printf("\n");
}

int main() {
    uint8_t msg1[] = "HELLO";          // 5 bytes (Incomplete block)
    uint8_t msg2[] = "12345678";       // 8 bytes (Exact block multiple)
    uint8_t padded[64], unpadded[64];
    int p_len, u_len;

    printf("=== Exp-21: Block Cipher 10* Padding Demonstration (ECB/CBC/CFB) ===\n\n");

    /* Case 1: Incomplete Block */
    printf("[Case 1: Message length = 5 bytes (Partial block)]\n");
    print_hex("Original", msg1, 5);
    p_len = apply_padding(msg1, 5, padded);
    print_hex("Padded  ", padded, p_len);
    u_len = remove_padding(padded, p_len, unpadded);
    printf("Unpadded: \"%s\" (Len: %d)\n\n", unpadded, u_len);

    /* Case 2: Exact Multiple of Block Size */
    printf("[Case 2: Message length = 8 bytes (Exact full block)]\n");
    print_hex("Original", msg2, 8);
    p_len = apply_padding(msg2, 8, padded);
    print_hex("Padded  ", padded, p_len);
    printf("Note: A full new dummy padding block [80 00 00 00 00 00 00 00] was appended!\n");
    u_len = remove_padding(padded, p_len, unpadded);
    printf("Unpadded: \"%s\" (Len: %d)\n\n", unpadded, u_len);

    printf("--- Question: What is the motivation for including a full padding block when not needed? ---\n");
    printf("Answer: Unambiguous Unpadding.\n");
    printf("If an unpadded message naturally ends with 0x80 00... or if we didn't pad full blocks,\n");
    printf("the receiver would have no way to know whether the final bytes are part of valid user data\n");
    printf("or padding bits. By ALWAYS adding padding (adding a full block if full), unpadding is 100%% deterministic.\n");

    return 0;
}
