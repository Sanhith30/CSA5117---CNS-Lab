#include <stdio.h>

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
    long long q = 353;
    long long a = 3;
    long long xA = 97, xB = 233;
    long long YA, YB, KA, KB;
    long long YA_flawed, YB_flawed;

    printf("=== Exp-28: Diffie-Hellman Key Exchange & Flawed Variant Analysis ===\n\n");

    /* 1. Standard Diffie-Hellman: YA = a^xA mod q, YB = a^xB mod q */
    YA = power_mod(a, xA, q);
    YB = power_mod(a, xB, q);

    KA = power_mod(YB, xA, q);
    KB = power_mod(YA, xB, q);

    printf("[1. Standard Diffie-Hellman Protocol]\n");
    printf("Public Parameters: Prime q = %lld, Generator a = %lld\n", q, a);
    printf("Alice secret xA = %lld -> Sends YA = a^xA mod q = %lld\n", xA, YA);
    printf("Bob   secret xB = %lld -> Sends YB = a^xB mod q = %lld\n", xB, YB);
    printf("Shared Key Alice: K = YB^xA mod q = %lld\n", KA);
    printf("Shared Key Bob  : K = YA^xB mod q = %lld (Shared match: %s)\n\n", KB, (KA == KB) ? "YES" : "NO");

    /* 2. Flawed Variant Analysis: Participants send x^a mod q */
    printf("[2. Flawed Variant: Sending x^a mod q instead of a^x mod q]\n");
    YA_flawed = power_mod(xA, a, q);
    YB_flawed = power_mod(xB, a, q);
    printf("Alice transmits: YA = xA^a mod q = %lld\n", YA_flawed);
    printf("Bob transmits  : YB = xB^a mod q = %lld\n\n", YB_flawed);

    printf("--- Analysis & Answers to Questions ---\n");
    printf("Q1: What happens if participants send x^a mod q instead of a^x mod q?\n");
    printf("    Answer: Key agreement becomes symmetric to computing modular a-th roots (like RSA encryption without secret trapdoor).\n");
    printf("    If gcd(a, q-1) = 1, then a has an inverse d = a^(-1) mod (q-1). Anyone, including Eve, can compute d\n");
    printf("    and compute (x^a)^d mod q = x directly!\n\n");

    printf("Q2: Can Eve break the system without finding the secret numbers?\n");
    printf("    Answer: If a shared key is (YA * YB) mod q = (xA * xB)^a mod q, Eve can compute it directly as YA * YB mod q.\n\n");

    printf("Q3: Can Eve find the secret numbers?\n");
    printf("    Answer: YES! When q is prime and a is public, finding the a-th root modulo q is easy using Euler's totient theorem:\n");
    printf("    d = a^(-1) mod (q-1), so x = (x^a)^d mod q.\n");

    return 0;
}
