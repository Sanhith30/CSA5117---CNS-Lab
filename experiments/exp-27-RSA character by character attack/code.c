#include <stdio.h>
#include <string.h>

long long mul_mod(long long a, long long b, long long mod) {
    long long res = 0;
    a %= mod;
    while (b > 0) {
        if (b % 2 == 1) res = (res + a) % mod;
        a = (a * 2) % mod;
        b /= 2;
    }
    return res;
}

long long power_mod(long long base, long long exp, long long mod) {
    long long res = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = mul_mod(res, base, mod);
        exp = exp / 2;
        base = mul_mod(base, base, mod);
    }
    return res;
}

int main() {
    long long e = 65537;
    long long n = 10403; // Even with large n, space is only 26
    char secret_message[] = "ATTACKATDAWN";
    int len = strlen(secret_message);
    long long ciphertext[100];
    long long lookup_table[26];
    char recovered_text[100];
    int i, ch;

    printf("=== Exp-27: RSA Character-by-Character Encryption Attack ===\n\n");
    printf("Public Key: e = %lld, n = %lld\n", e, n);
    printf("Alice's Secret Message: %s\n\n", secret_message);

    /* Alice encrypts character by character: C_i = M_i^e mod n */
    printf("Alice transmits ciphertext blocks:\n");
    for (i = 0; i < len; i++) {
        long long m = secret_message[i] - 'A'; // 0..25
        ciphertext[i] = power_mod(m, e, n);
        printf("%lld ", ciphertext[i]);
    }
    printf("\n\n");

    /* ATTACK: Attacker builds a dictionary / codebook of size 26 in O(1) */
    printf("--- Eve's Forward Search / Dictionary Attack ---\n");
    printf("Eve precomputes RSA encryption for all 26 possible plaintext characters (A-Z):\n");
    for (ch = 0; ch < 26; ch++) {
        lookup_table[ch] = power_mod(ch, e, n);
    }

    /* Eve instantly decrypts Alice's ciphertext by table lookup */
    for (i = 0; i < len; i++) {
        for (ch = 0; ch < 26; ch++) {
            if (ciphertext[i] == lookup_table[ch]) {
                recovered_text[i] = 'A' + ch;
                break;
            }
        }
    }
    recovered_text[len] = '\0';

    printf("Eve recovered plaintext: \"%s\"\n\n", recovered_text);
    printf("--- Analysis & Answers ---\n");
    printf("Is this method secure? NO, it is completely insecure!\n");
    printf("Why: The plaintext space is tiny (only 26 possible values). RSA is deterministic,\n");
    printf("so an attacker simply encrypts all 26 letters (0..25) with public key (e, n)\n");
    printf("and matches ciphertexts directly in O(26) time without factoring n or finding d.\n");
    printf("Fix: Use randomized padding (e.g., RSA-OAEP) and block sizes larger than modulus length.\n");

    return 0;
}
