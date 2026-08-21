#include <stdio.h>
#include <stdlib.h>

long long gcd(long long a, long long b) {
    while (b != 0) {
        long long t = b;
        b = a % b;
        a = t;
    }
    return a;
}

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
    long long p = 61, q = 53;
    long long n = p * q; // 3233
    long long e_old = 17;
    long long d_old = 2753; // Leaked old private key
    long long e_new = 23;   // Bob generates new e with same n!
    long long k, t, factor_found, a;
    long long recovered_phi, m, t_inv, q_div;
    long long x0, x1, a_inv, d_new_cracked;

    printf("=== Exp-26: RSA Leaked Private Key & Modulus Reuse Vulnerability ===\n\n");
    printf("Modulus (n) = %lld\n", n);
    printf("Old Public Key (e) = %lld, Leaked Private Key (d) = %lld\n", e_old, d_old);
    printf("New Public Key (e') chosen by Bob with same modulus n = %lld\n\n", e_new);

    printf("--- Attacker Factoring n using (e_old, d_old) ---\n");
    k = e_old * d_old - 1; // Multiple of phi(n): 17 * 2753 - 1 = 46800
    printf("k = e * d - 1 = %lld (multiple of phi(n))\n", k);

    // Factor n using randomized algorithm for e*d - 1
    t = k;
    while (t % 2 == 0) t /= 2;

    factor_found = 0;
    for (a = 2; a < 100; a++) {
        long long g = gcd(a, n);
        long long cur;
        if (g > 1 && g < n) {
            factor_found = g;
            break;
        }
        cur = power_mod(a, t, n);
        while (cur != 1 && cur != n - 1 && t < k) {
            long long next_cur = power_mod(cur, 2, n);
            if (next_cur == 1) {
                factor_found = gcd(cur - 1, n);
                break;
            }
            cur = next_cur;
        }
        if (factor_found > 1 && factor_found < n) break;
    }

    printf("Factor recovered: p = %lld, q = %lld\n", factor_found, n / factor_found);
    recovered_phi = (factor_found - 1) * ((n / factor_found) - 1);
    
    // Now attacker can compute Bob's NEW private key d_new!
    m = recovered_phi;
    x0 = 0;
    x1 = 1;
    a_inv = e_new;
    while (a_inv > 1) {
        q_div = a_inv / m;
        t_inv = m; m = a_inv % m; a_inv = t_inv;
        t_inv = x0; x0 = x1 - q_div * x0; x1 = t_inv;
    }
    if (x1 < 0) x1 += recovered_phi;
    d_new_cracked = x1;

    printf("Attacker computed Bob's NEW private key (d') = %lld\n\n", d_new_cracked);
    printf("CONCLUSION: Reusing modulus n after a private key leak is TOTALLY INSECURE.\n");
    printf("Anyone with (e, d) can easily factor n and compute any subsequent private keys.\n");

    return 0;
}
