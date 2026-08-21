#include <stdio.h>

/* Extended Euclidean Algorithm to find modular multiplicative inverse: (a * x) % m == 1 */
long long mod_inverse(long long a, long long m) {
    long long m0 = m, t, q;
    long long x0 = 0, x1 = 1;

    if (m == 1) return 0;

    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m;
        a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    if (x1 < 0) x1 += m0;
    return x1;
}

long long power_mod(long long base, long long exp, long long mod) {
    long long res = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % mod;
        exp = exp / 2;
        base = (base * base) % mod;
    }
    return res;
}

int main() {
    long long e = 31;
    long long n = 3599;
    long long p = 0, q = 0, phi = 0, d = 0;
    long long i, msg, cipher, decrypted;

    printf("=== Exp-24: RSA Private Key Calculation for e = 31, n = 3599 ===\n\n");

    /* Step 1: Factorize n = 3599 using trial division */
    for (i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            p = i;
            q = n / i;
            break;
        }
    }

    printf("1. Factorization of n = %ld:\n", (long)n);
    printf("   p = %ld, q = %ld\n\n", (long)p, (long)q);

    /* Step 2: Compute Euler's Totient phi(n) = (p-1)*(q-1) */
    phi = (p - 1) * (q - 1);
    printf("2. Euler's Totient phi(n) = (%ld - 1) * (%ld - 1) = %ld\n\n", (long)p, (long)q, (long)phi);

    /* Step 3: Compute private key d = e^(-1) mod phi(n) */
    d = mod_inverse(e, phi);
    printf("3. Multiplicative inverse of e = %ld modulo %ld:\n", (long)e, (long)phi);
    printf("   Private Key (d) = %ld\n\n", (long)d);

    /* Step 4: Verification with encryption & decryption */
    msg = 123;
    cipher = power_mod(msg, e, n);
    decrypted = power_mod(cipher, d, n);

    printf("--- Verification ---\n");
    printf("Original Message  : %ld\n", (long)msg);
    printf("Encrypted (C=M^e) : %ld\n", (long)cipher);
    printf("Decrypted (M=C^d) : %ld (Match: %s)\n", (long)decrypted, (msg == decrypted) ? "YES" : "NO");

    return 0;
}
