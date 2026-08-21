#include <stdio.h>
#include <string.h>

/* Modular inverse in Z_26 */
int mod_inv26(int a) {
    int x;
    a = (a % 26 + 26) % 26;
    for (x = 1; x < 26; x++) {
        if ((a * x) % 26 == 1) return x;
    }
    return -1;
}

/* Inverts a 2x2 matrix modulo 26 */
int invert_matrix_2x2(int m[2][2], int inv[2][2]) {
    int det = (m[0][0] * m[1][1] - m[0][1] * m[1][0]) % 26;
    int det_inv;
    det = (det + 26) % 26;
    det_inv = mod_inv26(det);
    if (det_inv == -1) return 0; // Matrix non-invertible

    inv[0][0] = (m[1][1] * det_inv) % 26;
    inv[0][1] = ((-m[0][1] + 260) * det_inv) % 26;
    inv[1][0] = ((-m[1][0] + 260) * det_inv) % 26;
    inv[1][1] = (m[0][0] * det_inv) % 26;
    return 1;
}

/* Multiplies two 2x2 matrices modulo 26: Res = A * B mod 26 */
void mat_mul_2x2(int a[2][2], int b[2][2], int res[2][2]) {
    int i, j;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            res[i][j] = (a[i][0] * b[0][j] + a[i][1] * b[1][j]) % 26;
        }
    }
}

int main() {
    /* Secret Key Matrix used by target */
    int K_secret[2][2] = {{9, 4}, {5, 7}};
    int P_known[2][2] = {{7, 8}, {4, 11}}; // e.g. "HE" (7, 4) and "IL" (8, 11)
    int C_known[2][2];
    int P_inv[2][2];
    int P_chosen[2][2] = {{1, 0}, {0, 1}};
    int C_chosen[2][2];

    printf("=== Exp-38: Known-Plaintext & Chosen-Plaintext Attack on 2x2 Hill Cipher ===\n\n");
    printf("Secret Key Matrix K:\n");
    printf("[ %2d  %2d ]\n[ %2d  %2d ]\n\n",
           K_secret[0][0], K_secret[0][1], K_secret[1][0], K_secret[1][1]);

    /* [1. Known-Plaintext Attack]
       Given 2 known plaintext blocks P = [P1 | P2] and corresponding ciphertext blocks C = [C1 | C2]
       Since C = K * P mod 26 ==> K = C * P^(-1) mod 26
    */
    mat_mul_2x2(K_secret, P_known, C_known);

    printf("[1. Known Plaintext Attack]\n");
    printf("Known Plaintext Matrix P:  [ %2d %2d ]\n                           [ %2d %2d ]\n",
           P_known[0][0], P_known[0][1], P_known[1][0], P_known[1][1]);
    printf("Corresponding Cipher C:    [ %2d %2d ]\n                           [ %2d %2d ]\n\n",
           C_known[0][0], C_known[0][1], C_known[1][0], C_known[1][1]);

    if (invert_matrix_2x2(P_known, P_inv)) {
        int K_recovered[2][2];
        mat_mul_2x2(C_known, P_inv, K_recovered);
        printf("Attacker calculates K = C * P^(-1) mod 26:\n");
        printf("Recovered Key Matrix:\n");
        printf("[ %2d  %2d ]\n[ %2d  %2d ] (Match: %s)\n\n",
               K_recovered[0][0], K_recovered[0][1], K_recovered[1][0], K_recovered[1][1],
               (K_recovered[0][0] == K_secret[0][0] && K_recovered[1][1] == K_secret[1][1]) ? "YES" : "NO");
    }

    /* [2. Chosen-Plaintext Attack]
       Attacker simply submits the identity matrix P_chosen = I = [[1, 0], [0, 1]]
       Then C_chosen = K * I = K !
    */
    printf("[2. Chosen Plaintext Attack]\n");
    printf("Attacker chooses Identity Matrix P = [[1, 0], [0, 1]] (Plaintext \"BA\" and \"AB\")\n");
    mat_mul_2x2(K_secret, P_chosen, C_chosen);

    printf("Received Ciphertext C = K * I directly yields Key K:\n");
    printf("[ %2d  %2d ]\n[ %2d  %2d ]\n\n",
           C_chosen[0][0], C_chosen[0][1], C_chosen[1][0], C_chosen[1][1]);
    printf("Conclusion: Chosen-plaintext attack recovers Hill cipher key with ZERO inversions.\n");

    return 0;
}
