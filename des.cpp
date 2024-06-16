#include <stdio.h> 

#include <stdint.h> 

typedef uint64_t block_t; 

typedef uint64_t key_t; 

// Initial permutation (IP) table for 64-bit blocks 

static const int IP[] = { 

    57, 49, 41, 33, 25, 17, 9, 1, 

    59, 51, 43, 35, 27, 19, 11, 3, 

    61, 53, 45, 37, 29, 21, 13, 5, 

    63, 55, 47, 39, 31, 23, 15, 7, 

    56, 48, 40, 32, 24, 16, 8, 0, 

    58, 50, 42, 34, 26, 18, 10, 2, 

    60, 52, 44, 36, 28, 20, 12, 4, 

    62, 54, 46, 38, 30, 22, 14, 6 

}; 
static const int FP[] = { 

    39, 7, 47, 15, 55, 23, 63, 31, 

    38, 6, 46, 14, 54, 22, 62, 30, 

    37, 5, 45, 13, 53, 21, 61, 29, 

    36, 4, 44, 12, 52, 20, 60, 28, 

    35, 3, 43, 11, 51, 19, 59, 27, 

    34, 2, 42, 10, 50, 18, 58, 26, 

    33, 1, 41, 9, 49, 17, 57, 25, 

    32, 0, 40, 8, 48, 16, 56, 24 

}; 
static const int E[] = { 

    31, 0, 1, 2, 3, 4, 

    3, 4, 5, 6, 7, 8, 

    7, 8, 9, 10, 11, 12, 

    11, 12, 13, 14, 15, 16, 

    15, 16, 17, 18, 19, 20, 

    19, 20, 21, 22, 23, 24, 

    23, 24, 25, 26, 27, 28, 

    27, 28, 29, 30, 31, 0 

}; 
static const uint8_t SBOX[8][4][16] = { 

    { 

        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7}, 

        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8}, 

        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0}, 

        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13} 

    }, 

    { 

        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10}, 

        {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5}, 

        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15}, 

        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1} 

    }, 

    { 

        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8}, 

        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1}, 

        {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7}, 

        {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12} 

    }, 

    { 

        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15}, 

        {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9}, 

        {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4}, 

        {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14} 

    }, 

    { 

        {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9}, 

        {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6}, 

        {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14}, 

        {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3} 

    }, 

    { 

        {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11}, 

        {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8}, 

        {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6}, 

        {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13} 

    }, 

    { 

        {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1}, 

        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6}, 

        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2}, 

        {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12} 

    }, 
    { 

        {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7}, 

        {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2}, 

        {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8}, 

        {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11} 

    } 

}; 
static const int P[] = { 

    15, 6, 19, 20, 28, 11, 27, 16, 

    0, 14, 22, 25, 4, 17, 30, 9, 

    1, 7, 23, 13, 31, 26, 2, 8, 

    18, 12, 29, 5, 21, 10, 3, 24 

}; 

 
 static const int PC1[] = { 

    56, 48, 40, 32, 24, 16, 8, 

    0, 57, 49, 41, 33, 25, 17, 

    9, 1, 58, 50, 42, 34, 26, 

    18, 10, 2, 59, 51, 43, 35, 

    62, 54, 46, 38, 30, 22, 14, 

    6, 61, 53, 45, 37, 29, 21, 

    13, 5, 60, 52, 44, 36, 28, 

    20, 12, 4 

}; 
static const int PC2[] = { 

    13, 16, 10, 23, 0, 4, 

    2, 27, 14, 5, 20, 9, 

    22, 18, 11, 3, 25, 7, 

    15, 6, 26, 19, 12, 1, 

    40, 51, 30, 36, 46, 54, 

    29, 39, 50, 44, 32, 47, 

    43, 48, 38, 55, 33, 52, 

    45, 41, 49, 35, 28, 31 

}; 
static const int SHIFTS[] = { 

    1, 1, 2, 2, 2, 2, 2, 2, 

    1, 2, 2, 2, 2, 2, 2, 1 

}; 
uint32_t left_shift(uint32_t value, int shift) { 

    return (value << shift) | (value >> (28 - shift)); 

} 

 

// Perform initial permutation (IP) on 64-bit block 

void initial_permutation(block_t *block) { 

    block_t result = 0; 

    for (int i = 0; i < 64; i++) { 

        int shift = 63 - IP[i]; 

        if (*block & ((block_t)1 << shift)) { 

            result |= ((block_t)1 << i); 

        } 

    } 

    *block = result; 

} 
void final_permutation(block_t *block) { 

    block_t result = 0; 

    for (int i = 0; i < 64; i++) { 

        int shift = 63 - FP[i]; 

        if (*block & ((block_t)1 << shift)) { 

            result |= ((block_t)1 << i); 

        } 

    } 

    *block = result; 

} 
void expansion_permutation(uint32_t *block) { 

    uint32_t result = 0; 

    for (int i = 0; i < 48; i++) { 

        int shift = 31 - E[i]; 

        if (*block & (1 << shift)) { 

            result |= (1 << i); 

        } 

    } 

    *block = result; 

} 
void substitution(uint32_t *block) { 

    uint32_t result = 0; 

    for (int i = 0; i < 8; i++) { 

        uint8_t row = ((*block) & 0x0000840000000000) >> 42 | ((*block) & 0x0000010000000000) >> 1; 

        uint8_t col = ((*block) & 0x0000780000000000) >> 43; 

        *block <<= 6; 

        *block |= SBOX[i][row][col]; 

    } 

} 
void permutation(uint32_t *block) { 

    uint32_t result = 0; 

    for (int i = 0; i < 32; i++) { 

        int shift = 31 - P[i]; 

        if (*block & (1 << shift)) { 

            result |= (1 << i); 

        } 

    } 

    *block = result; 

} 
void permuted_choice1(key_t key, uint32_t *C, uint32_t *D) { 

    *C = *D = 0; 

    for (int i = 0; i < 28; i++) { 

        int shift = 63 - PC1[i]; 

        if (key & ((key_t)1 << shift)) { 

            *C |= ((uint32_t)1 << i); 

        } 

    } 

    for (int i = 28; i < 56; i++) { 

        int shift = 63 - PC1[i]; 

        if (key & ((key_t)1 << shift)) { 

            *D |= ((uint32_t)1 << (i - 28)); 

        } 

    } 

} 
void permuted_choice2(uint32_t C, uint32_t D, uint64_t *K) { 

    *K = 0; 

    uint64_t CD = ((uint64_t)C << 28) | D; 

    for (int i = 0; i < 48; i++) { 

        int shift = 55 - PC2[i]; 

        if (CD & ((uint64_t)1 << shift)) { 

            *K |= ((uint64_t)1 << i); 

        } 

    } 

} 
void key_schedule(key_t key, uint64_t *K) { 

    uint32_t C, D; 

    permuted_choice1(key, &C, &D); 

    for (int i = 0; i < 16; i++) { 

        C = left_shift(C, SHIFTS[i]); 

        D = left_shift(D, SHIFTS[i]); 

        permuted_choice2(C, D, &K[i]); 

    } 

} 
void feistel_encrypt(uint32_t *L, uint32_t *R, uint64_t K) { 

    uint32_t R_expanded = *R; 

    expansion_permutation(&R_expanded); 

    R_expanded ^= K; 

    substitution(&R_expanded); 

    permutation(&R_expanded); 

    *L ^= R_expanded; 

} 
void feistel_decrypt(uint32_t *L, uint32_t *R, uint64_t K) { 

    uint32_t R_expanded = *R; 

    expansion_permutation(&R_expanded); 

    R_expanded ^= K; 

    substitution(&R_expanded); 

    permutation(&R_expanded); 

    *L ^= R_expanded; 

} 
void des_encrypt(block_t *block, key_t key) { 

    initial_permutation(block); 

     

    uint32_t L = *block >> 32; 

    uint32_t R = *block; 

     

    uint64_t K[16]; 

    key_schedule(key, K); 

     

    for (int i = 0; i < 16; i++) { 

        feistel_encrypt(&L, &R, K[i]); 

    } 
    *block = ((block_t)R << 32) | L; 

     

    final_permutation(block); 

} 
void des_decrypt(block_t *block, key_t key) { 

    initial_permutation(block); 

     

    uint32_t L = *block >> 32; 

    uint32_t R = *block; 

     

    uint64_t K[16]; 

    key_schedule(key, K); 

     

    for (int i = 15; i >= 0; i--) { 

        feistel_decrypt(&L, &R, K[i]); 

    } 

     

    *block = ((block_t)R << 32) | L; 
    final_permutation(block); 

} 

 

// Print a 64-bit block in hexadecimal format 

void print_block(block_t block) { 

    printf("%016llx\n", block); 

} 

 

int main() { 
printf("NEETU JOSHI\n");

    // Example plaintext and key 

    block_t plaintext = 0x0123456789ABCDEF; 

    key_t key = 0x133457799BBCDFF1; 

     

    printf("Plaintext:\n"); 

    print_block(plaintext); 

     

    printf("Key:\n"); 

    print_block(key); 

     

    // Encryption 

    des_encrypt(&plaintext, key); 

    printf("After encryption:\n"); 

    print_block(plaintext); 

     

    // Decryption 

    des_decrypt(&plaintext, key); 

    printf("After decryption:\n"); 

    print_block(plaintext); 

     

    return 0; 

} 
