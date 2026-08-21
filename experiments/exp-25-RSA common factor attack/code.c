#include <stdio.h>

long long gcd(long long a, long long b) {
    while (b != 0) {
        long long t = b;
        b = a % b;
        a = t;
    }
    return a;
}

long long mod_inverse(long long a, long long m) {
    long long m0 = m, t, q;
    long long x0 = 0, x1 = 1;
    if (m == 1) return 0;
    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m, a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0) x1 += m0;
    return x1;
}

int main() {
    long long p_secret = 101;
    long long q_secret = 113;
    long long n = p_secret * q_secret; // 11413
    long long e = 65537;

    // Suppose one plaintext block M shares a factor with n (e.g. M = 5 * p = 505)
    long long M_known_factor = 505;

    printf("=== Exp-25: RSA Common Factor Attack Demonstration ===\n\n");
    printf("Public Modulus (n) = %lld\n", n);
    printf("Public Exponent (e) = %lld\n", e);
    printf("Plaintext block with common factor with n: M = %lld\n\n", M_known_factor);

    long long factor = gcd(M_known_factor, n);

    printf("--- Execution of Attack ---\n");
    printf("Computing gcd(M, n) = gcd(%lld, %lld)...\n", M_known_factor, n);
    printf("Result: gcd(M, n) = %lld\n\n", factor);

    if (factor > 1 && factor < n) {
        long long p = factor;
        long long q = n / factor;
        long long phi = (p - 1) * (q - 1);
        long long d = mod_inverse(e, phi);

        printf("CRITICAL VULNERABILITY FOUND:\n");
        printf("1. Factor p = %lld\n", p);
        printf("2. Factor q = %lld\n", q);
        printf("3. phi(n)   = (%lld - 1) * (%lld - 1) = %lld\n", p, q, phi);
        printf("4. Recovered Private Key (d) = %lld\n\n", d);
        printf("Conclusion: YES! If any plaintext block M shares a non-trivial factor with n,\n");
        printf("gcd(M, n) directly yields one of the prime factors (p or q),\n");
        printf("completely breaking the RSA cryptosystem for all messages.\n");
    } else {
        printf("No non-trivial factor found.\n");
    }

    return 0;
}
