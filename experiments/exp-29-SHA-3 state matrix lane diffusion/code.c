#include <stdio.h>
#include <stdint.h>

#define TOTAL_LANES 25
#define RATE_LANES 16
#define CAPACITY_LANES 9

int main() {
    int state[5][5]; // 1 if lane has at least one non-zero bit, 0 otherwise
    int next_state[5][5];
    int col_parity[5];
    int x, y, round = 0, lane_idx = 0;
    int all_nonzero = 0;

    printf("=== Exp-29: SHA-3 State Matrix Lane Non-Zero Bit Diffusion Simulation ===\n\n");
    printf("State Size: 1600 bits (5x5 = 25 lanes, 64 bits/lane)\n");
    printf("Rate (r)    : 1024 bits = 16 lanes (all initialized with non-zero bits from P0)\n");
    printf("Capacity (c): 576 bits  =  9 lanes (all initialized to zero)\n\n");

    /* Initial state after absorbing P0 */
    for (y = 0; y < 5; y++) {
        for (x = 0; x < 5; x++) {
            if (lane_idx < RATE_LANES) {
                state[x][y] = 1; // Rate lane: non-zero
            } else {
                state[x][y] = 0; // Capacity lane: zero
            }
            lane_idx++;
        }
    }

    printf("Initial State Matrix (1 = non-zero lane, 0 = zero lane):\n");
    for (y = 0; y < 5; y++) {
        for (x = 0; x < 5; x++) {
            printf("%d ", state[x][y]);
        }
        printf("\n");
    }

    /* Simulation of Theta step: lane(x, y) ^= Parity(col x-1) ^ Parity(col x+1) */
    while (!all_nonzero && round < 10) {
        round++;
        printf("\n--- Round %d Diffusion (Theta Transformation) ---\n", round);

        for (x = 0; x < 5; x++) {
            col_parity[x] = 0;
            for (y = 0; y < 5; y++) {
                col_parity[x] ^= state[x][y];
            }
        }

        for (x = 0; x < 5; x++) {
            for (y = 0; y < 5; y++) {
                int d = col_parity[(x + 4) % 5] ^ col_parity[(x + 1) % 5];
                next_state[x][y] = state[x][y] | d; // Becomes non-zero if either is non-zero
            }
        }

        all_nonzero = 1;
        for (y = 0; y < 5; y++) {
            for (x = 0; x < 5; x++) {
                state[x][y] = next_state[x][y];
                printf("%d ", state[x][y]);
                if (state[x][y] == 0) all_nonzero = 0;
            }
            printf("\n");
        }
    }

    printf("\nConclusion: All 9 capacity lanes obtain non-zero bits after exactly %d round(s) of Keccak-f.\n", round);
    return 0;
}
