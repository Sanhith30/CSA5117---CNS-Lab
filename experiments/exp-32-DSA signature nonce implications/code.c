#include <stdio.h>
#include <stdlib.h>

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

long long mod_inv(long long a, long long m) {
    long long m0 = m, t, q;
    long long x0 = 0, x1 = 1;
    if (m == 1) return 0;
    while (a > 1) {
        q = a / m;
        t = m; m = a % m; a = t;
        t = x0; x0 = x1 - q * x0; x1 = t;
    }
    if (x1 < 0) x1 += m0;
    return x1;
}

int main() {
    /* DSA domain parameters */
    long long p = 283, q = 47, g = 60; // g = h^((p-1)/q) mod p
    long long x = 24; // Private signing key
    long long H_M = 19; // Hash of message M
    long long k1, r1, s1;
    long long k2, r2, s2;
    long long n_rsa, d_rsa, sig_rsa1, sig_rsa2;
    long long H_M1, H_M2, k_reused, r_reused, s_m1, s_m2;
    long long s_diff, h_diff, k_recovered, x_recovered;

    printf("=== Exp-32: DSA vs RSA Nonce & Signature Randomization Implications ===\n\n");
    printf("DSA Parameters: p = %lld, q = %lld, g = %lld, Private Key x = %lld\n", p, q, g, x);
    printf("Message Hash H(M) = %lld\n\n", H_M);

    /* 1. Signing same message with two different per-message nonces k1, k2 */
    k1 = 15;
    r1 = power_mod(g, k1, p) % q;
    s1 = (mod_inv(k1, q) * (H_M + x * r1)) % q;

    k2 = 23;
    r2 = power_mod(g, k2, p) % q;
    s2 = (mod_inv(k2, q) * (H_M + x * r2)) % q;

    printf("[1. DSA Randomized Signatures for the exact same message]\n");
    printf("Signature 1 (with k=%lld): (r1 = %lld, s1 = %lld)\n", k1, r1, s1);
    printf("Signature 2 (with k=%lld): (r2 = %lld, s2 = %lld)\n\n", k2, r2, s2);

    /* 2. Deterministic RSA signature for contrast */
    n_rsa = 3233; d_rsa = 2753;
    sig_rsa1 = power_mod(H_M, d_rsa, n_rsa);
    sig_rsa2 = power_mod(H_M, d_rsa, n_rsa);
    printf("[2. RSA Deterministic Signature for the exact same message]\n");
    printf("Signature 1: %lld | Signature 2: %lld (Always identical)\n\n", sig_rsa1, sig_rsa2);

    /* 3. CATASTROPHIC IMPLICATION: Nonce reuse attack in DSA */
    printf("[3. Catastrophic Implication of DSA Nonce: Private Key Leak via Nonce Reuse]\n");
    H_M1 = 19; H_M2 = 31;
    k_reused = 15;
    r_reused = power_mod(g, k_reused, p) % q;
    s_m1 = (mod_inv(k_reused, q) * (H_M1 + x * r_reused)) % q;
    s_m2 = (mod_inv(k_reused, q) * (H_M2 + x * r_reused)) % q;

    printf("Signer signed 2 DIFFERENT messages (H1=%lld, H2=%lld) with SAME nonce k=%lld:\n", H_M1, H_M2, k_reused);
    printf("Sig 1: (r=%lld, s=%lld), Sig 2: (r=%lld, s=%lld)\n\n", r_reused, s_m1, r_reused, s_m2);

    // Attacker recovers k: k = (H1 - H2) / (s1 - s2) mod q
    s_diff = (s_m1 - s_m2 + q) % q;
    h_diff = (H_M1 - H_M2 + q) % q;
    k_recovered = (h_diff * mod_inv(s_diff, q)) % q;

    // Attacker recovers private key x: x = (s1 * k - H1) * r^(-1) mod q
    x_recovered = (((s_m1 * k_recovered - H_M1) % q + q) % q * mod_inv(r_reused, q)) % q;

    printf("Attacker recovered nonce k       : %lld (Actual: %lld)\n", k_recovered, k_reused);
    printf("Attacker recovered PRIVATE KEY x : %lld (Actual: %lld)\n\n", x_recovered, x);

    printf("Conclusion: In DSA, per-message nonce k makes signatures non-deterministic and protects\n");
    printf("against replay identification, BUT reusing or weakly generating k completely exposes the private key!\n");

    return 0;
}
