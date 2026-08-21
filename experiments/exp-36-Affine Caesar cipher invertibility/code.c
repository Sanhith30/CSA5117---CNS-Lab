#include <stdio.h>

int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int mod_inverse(int a, int m) {
    int x;
    for (x = 1; x < m; x++) {
        if ((a * x) % m == 1) return x;
    }
    return -1;
}

int main() {
    int a, b = 3, p;
    int c0, c13, valid_count;

    printf("=== Exp-36: Affine Caesar Cipher Invertibility / Bijectivity Analysis ===\n\n");
    printf("Encryption Formula: C = (a * p + b) mod 26\n");
    printf("Fixed shift b = %d\n\n", b);

    /* 1. Demonstrate non one-to-one mapping for a = 2 */
    printf("[1. Demonstrating collision / non one-to-one mapping for a = 2, b = 3]:\n");
    c0 = (2 * 0 + 3) % 26;   // p = 0 ('A')
    c13 = (2 * 13 + 3) % 26; // p = 13 ('N')
    printf("   p = 0  ('A') -> C = (2 * 0 + 3) mod 26  = %d ('%c')\n", c0, 'A' + c0);
    printf("   p = 13 ('N') -> C = (2 * 13 + 3) mod 26 = %d ('%c')\n", c13, 'A' + c13);
    printf("   Collision! Both 'A' and 'N' map to 'D' (3). Decryption is impossible.\n\n");

    /* 2. Comprehensive check for all values of a from 0 to 25 */
    printf("[2. Testing all multiplicative keys 'a' in range [0, 25]]:\n");
    printf(" a | gcd(a, 26) | One-to-One (Bijection)? | Modular Inverse a^(-1) mod 26\n");
    printf("---+------------+-------------------------+------------------------------\n");

    valid_count = 0;
    for (a = 0; a < 26; a++) {
        int g = gcd(a, 26);
        int seen[26] = {0};
        int is_one_to_one = 1;

        for (p = 0; p < 26; p++) {
            int c = (a * p + b) % 26;
            if (seen[c]) {
                is_one_to_one = 0;
                break;
            }
            seen[c] = 1;
        }

        int inv = mod_inverse(a, 26);
        printf("%2d |     %2d     |          %s          | ",
               a, g, is_one_to_one ? "YES" : " NO");
        if (inv != -1) {
            printf("             %2d\n", inv);
            valid_count++;
        } else {
            printf("            None\n");
        }
    }

    printf("\nConclusion: The Affine Caesar Cipher is one-to-one IF AND ONLY IF gcd(a, 26) = 1.\n");
    printf("There are exactly %d valid values of 'a' in Z_26: {1, 3, 5, 7, 9, 11, 15, 17, 19, 21, 23, 25}.\n", valid_count);

    return 0;
}
