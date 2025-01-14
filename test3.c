#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define m (5)
#define n ((1 << m) - 1) // 2^m - 1
#define k 11
#define t ((n - k) / 2)

int gf_exp[n * 2];
int gf_log[n + 1];
int alpha = 2; // Primitive element

// Galois field initialization
void init_gf() {
    int x = 1;
    for (int i = 0; i < n; i++) {
        gf_exp[i] = x;
        gf_log[x] = i;
        x <<= 1;
        if (x & (1 << m)) x ^= 0x25; // Changed polynomial to x^5 + x^2 + 1
    }
    for (int i = n; i < 2 * n; i++) {
        gf_exp[i] = gf_exp[i - n];
    }
    gf_log[0] = -1;
}

int gf_mul(int a, int b) {
    return (a == 0 || b == 0) ? 0 : gf_exp[(gf_log[a] + gf_log[b]) % n];
}

int gf_inv(int a) {
    return gf_exp[n - gf_log[a]];
}

// Syndrome calculation
void calculate_syndromes(int *recv, int *syndromes) {
    for (int i = 0; i < 2 * t; i++) {
        syndromes[i] = 0;
        for (int j = 0; j < n; j++) {
            syndromes[i] ^= gf_mul(recv[j], gf_exp[(i * j) % n]);
        }
    }
}

// Berlekamp-Massey algorithm
void berlekamp_massey(int *syndromes, int *Lambda) {
    int L = 0, m2 = 1, b = 1;
    int C[2 * t + 1] = {1};
    int B[2 * t + 1] = {1};

    for (int n2 = 0; n2 < 2 * t; n2++) {
        int delta = syndromes[n2];
        for (int i = 1; i <= L; i++) {
            delta ^= gf_mul(C[i], syndromes[n2 - i]);
        }

        if (delta != 0) {
            int T[2 * t + 1];
            memcpy(T, C, sizeof(C));
            int delta_inv = gf_inv(b);
            for (int i = 0; i <= 2 * t; i++) {
                if (B[i] != 0) {
                    C[i + m2] ^= gf_mul(delta, B[i]);
                }
            }
            if (2 * L <= n2) {
                L = n2 + 1 - L;
                memcpy(B, T, sizeof(T));
                b = delta;
                m2 = 1;
            } else {
                m2++;
            }
        } else {
            m2++;
        }
    }
    memcpy(Lambda, C, sizeof(C));
}

// Chien search
int chien_search(int *Lambda, int *error_pos) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        int eval = 0;
        for (int j = 0; j <= t; j++) {
            eval ^= gf_mul(Lambda[j], gf_exp[(j * i) % n]);
        }
        if (eval == 0) {
            error_pos[count++] = n - 1 - i;
        }
    }
    return count;
}

// Main decoding function
void decode_rs(int *recv) {
    int syndromes[2 * t];
    int Lambda[2 * t + 1];
    int error_pos[t];

    calculate_syndromes(recv, syndromes);

    int all_zero = 1;
    for (int i = 0; i < 2 * t; i++) {
        if (syndromes[i] != 0) {
            all_zero = 0;
            break;
        }
    }
    if (all_zero) {
        printf("No errors detected.\n");
        return;
    }

    berlekamp_massey(syndromes, Lambda);
    int error_count = chien_search(Lambda, error_pos);

    if (error_count == 0) {
        printf("Decoding failed.\n");
        return;
    }

    printf("Error positions: ");
    for (int i = 0; i < error_count; i++) {
        printf("%d ", error_pos[i]);
    }
    printf("\n");
}

/*int main() {
    init_gf();
    int recv[n] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 22, 30, 5, 29, 10, 13, 11, 13, 29, 23, 19, 24, 12, 4, 22, 0, 28, 12, 25, 24};
    decode_rs(recv);
    return 0;
}*/