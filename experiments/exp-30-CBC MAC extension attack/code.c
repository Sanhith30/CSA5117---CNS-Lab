#include <stdio.h>
#include <stdint.h>

/* Simple block cipher mock for E_K */
uint64_t mock_encrypt(uint64_t block, uint64_t key) {
    return (block ^ key) * 0x5851F42D4C957F2DULL + 0x14057B7EF767814FULL;
}

/* CBC-MAC function for N blocks with IV = 0 */
uint64_t cbc_mac(const uint64_t *blocks, int n, uint64_t key) {
    uint64_t mac = 0; // IV = 0
    int i;
    for (i = 0; i < n; i++) {
        mac = mock_encrypt(mac ^ blocks[i], key);
    }
    return mac;
}

void print_hex64(uint64_t val) {
    printf("%08X%08X", (uint32_t)(val >> 32), (uint32_t)(val & 0xFFFFFFFF));
}

int main() {
    uint64_t key = 0x0123456789ABCDEFULL;
    uint64_t X = 0x4141414141414141ULL; // Single-block message "AAAAAAAA"
    uint64_t T, forged_mac;
    uint64_t forged_message[2];

    printf("=== Exp-30: CBC-MAC Extension Attack / Forgery Demonstration ===\n\n");
    printf("Secret Key (unknown to adversary): 0x"); print_hex64(key); printf("\n");
    printf("1-Block Message X                 : 0x"); print_hex64(X); printf("\n\n");

    /* Legitimate single-block MAC */
    T = cbc_mac(&X, 1, key);
    printf("Step 1: Adversary queries/intercepts MAC for X:\n");
    printf("        T = CBC_MAC(K, X) = 0x"); print_hex64(T); printf("\n\n");

    /* Adversary creates 2-block forgery without knowing K:
       Block 1 = X
       Block 2 = X XOR T
    */
    forged_message[0] = X;
    forged_message[1] = X ^ T;

    printf("Step 2: Adversary constructs 2-block message: M = X || (X ^ T)\n");
    printf("        Block 1: 0x"); print_hex64(forged_message[0]); printf("\n");
    printf("        Block 2: 0x"); print_hex64(forged_message[1]); printf("\n\n");

    /* Verification of MAC on forged message */
    forged_mac = cbc_mac(forged_message, 2, key);
    printf("Step 3: Calculating CBC-MAC on forged 2-block message:\n");
    printf("        Round 1: C1 = E_K(0 ^ X) = T = 0x"); print_hex64(T); printf("\n");
    printf("        Round 2: C2 = E_K(C1 ^ Block2) = E_K(T ^ (X ^ T)) = E_K(X) = T = 0x"); print_hex64(forged_mac); printf("\n\n");

    printf("--- Result ---\n");
    printf("Original MAC T        : 0x"); print_hex64(T); printf("\n");
    printf("Forged 2-block MAC    : 0x"); print_hex64(forged_mac); printf("\n");
    printf("MAC values identical? : %s\n\n", (T == forged_mac) ? "YES (FORGERY SUCCESSFUL!)" : "NO");

    printf("Explanation: CBC-MAC is insecure for variable-length messages without length-prepending or CMAC subkeys.\n");

    return 0;
}
