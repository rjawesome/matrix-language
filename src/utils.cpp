#include "utils.h"

int operating_line = 0;

int gcd(int a, int b) {
    a = abs(a);
    b = abs(b);
    if (!a) return b;
    if (!b) return a;

    // both numbers even
    int k = 0;
    while (!(a & 1) && !(b & 1)) {
        a >>= 1;
        b >>= 1;
        k++;
    }

    // "a" = designated odd number
    while (!(a & 1)) {
        a >>= 1;
    }

    while (b) {
        while (!(b & 1)) b >>= 1;
        if (a > b) {
            int t = a;
            a = b;
            b = t;
        }
        b -= a;
    }
    
    return a << k;
}


void print_m256(__m256i vec) {
    int bleh[8];
    _mm256_storeu_si256((__m256i*)(bleh), vec);
    for (int i = 0; i < 8; ++i) {
        // Extract each 32-bit integer from the _m256 register
        int value = bleh[i];
        std::cout << "Element " << i << ": " << value << std::endl;
    }
}

__m256i gcd_simd(__m256i a, __m256i b) {
    a = _mm256_abs_epi32(a);
    b = _mm256_abs_epi32(b);

    // make both numbers even
    __m256i k = _mm256_setzero_si256();
    while (true) {
        __m256i mask = _mm256_xor_si256(_mm256_and_si256(_mm256_or_si256(a, b), _mm256_set1_epi32(1)), _mm256_set1_epi32(1));
        if (_mm256_testz_si256(mask, mask)) break; // no more things to do
        a = _mm256_srlv_epi32(a, mask);
        b = _mm256_srlv_epi32(b, mask);
        k = _mm256_add_epi32(k, mask);
    }

    // "a" = designated odd number
    while (true) {
        __m256i mask = _mm256_xor_si256(_mm256_and_si256(a, _mm256_set1_epi32(1)), _mm256_set1_epi32(1));
        if (_mm256_testz_si256(mask, mask)) break; // no more things to do
        a = _mm256_srlv_epi32(a, mask);
    }

    while (true) {
        if (_mm256_testz_si256(a, a)) break;
        __m256i zeroMask = _mm256_and_si256(_mm256_cmpeq_epi32(b, _mm256_setzero_si256()), _mm256_set1_epi32(1));
        while (true) {
            __m256i mask = _mm256_xor_si256(_mm256_or_si256(_mm256_and_si256(b, _mm256_set1_epi32(1)), zeroMask), _mm256_set1_epi32(1));
            if (_mm256_testz_si256(mask, mask)) break; // no more things to do
            b = _mm256_srlv_epi32(b, mask);
        }
        __m256i t = a;
        a = _mm256_min_epi32(a, b);
        b = _mm256_max_epi32(t, b);
        b = _mm256_sub_epi32(b, a);
    }

    return _mm256_sllv_epi32(b, k);
}

// __m512i gcd_simd_512(__m512i a, __m512i b) {
//     _mm512_abs_epi32(a);
//     _mm512_abs_epi32(b);

//     // make both numbers even
//     __m512i k = _mm512_setzero_si512();
//     while (true) {
//         __mmask16 mask = _mm512_testn_epi32_mask(a, _mm512_set1_epi32(1)) & _mm512_testn_epi32_mask(b, _mm512_set1_epi32(1));
//         if (mask == 0) break; // no more things to do
//         a = _mm512_mask_srli_epi32(a, mask, a, 1);
//         b = _mm512_mask_srli_epi32(b, mask, b, 1);
//         k = _mm512_mask_add_epi32(k, mask, k, _mm512_set1_epi32(1));
//     }

//     // "a" = designated odd number
//     while (true) {
//         __mmask16 mask = _mm512_testn_epi32_mask(a, _mm512_set1_epi32(1));
//         if (mask == 0) break; // no more things to do
//         a = _mm512_mask_srli_epi32(a, mask, a, 1);
//     }

//     while (true) {
//         if (_mm512_test_epi32_mask(b, _mm512_setzero_si512()) == 0xFFFF) break;
//         while (true) {
//             __mmask16 mask = _mm512_testn_epi32_mask(b, _mm512_set1_epi32(1));
//             if (mask == 0) break; // no more things to do
//             b = _mm512_mask_srli_epi32(b, mask, b, 1);
//         }
//         __m512i t = a;
//         a = _mm512_min_epi32(a, b);
//         b = _mm512_max_epi32(t, b);
//         b = _mm512_sub_epi32(b, a);
//     }

//     return _mm512_sllv_epi32(a, k);
// }

pair<int, int> reduce_sqrt(int s) {
    int whole = 1;
    for (int i = 2; i <= sqrt(s); i++) {
        int sq = (int)pow(i, 2);
        if (s % sq == 0) {
            whole *= i;
            s /= sq;
        }
    }
    return {whole, s};
}

pair<int, int> reduce_sqrts(int s1, int s2) {
    int g = gcd(s1, s2);
    return {g, s1 * s2 / (g*g)};
}

void performance_test() {
    // Two arrays: One for each input vector of 16 integers
    int data_a[16] = {12, 7, 18, 36, 54, 42, 20, 80, 15, 35, 24, 60, 27, 14, 10, 90};
    int data_b[16] = {15, 35, 24, 60, 27, 14, 10, 90, 12, 7, 18, 36, 54, 42, 20, 80};

    // Timing for scalar GCD function
    clock_t start, end;
    double cpu_time_used_1, cpu_time_used_2;
    int result1[16][16];
    int result2[16][16];

    // SIMD GCD function
    start = clock();
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 2; j++) {
            int t[8];
            for (int k = 0; k < 8; k++) t[k] = data_a[i];
            __m256i a = _mm256_loadu_si256((__m256i*)(t));
            __m256i b = _mm256_loadu_si256((__m256i*)(data_b+(j*8)));
            __m256i result_vec = gcd_simd(a, b);
            _mm256_storeu_si256((__m256i*)(result1[i]+(j*8)), result_vec);
        }
    }
    end = clock();
    cpu_time_used_1 = ((double) (end - start)) / CLOCKS_PER_SEC;
    
    // Extract results and print
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            printf("gcd_simd(%d, %d) = %d\n", data_a[i], data_b[j], result1[i][j]);
        }
    }

    start = clock();
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            result2[i][j] = gcd(data_a[i], data_b[j]);
        }
    }
    end = clock();
    cpu_time_used_2 = ((double) (end - start)) / CLOCKS_PER_SEC;
    // Extract results and print
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            printf("gcd_simd(%d, %d) = %d\n", data_a[i], data_b[j], result2[i][j]);
            if (!(result1[i][j] == result2[i][j])) {
                exit(1);
            }
        }
    }
    printf("SIMD GCD function took %f seconds\n", cpu_time_used_1);
    printf("Scalar GCD function took %f seconds\n", cpu_time_used_2);
    printf("is simd less than scalar: %d\n", cpu_time_used_1 < cpu_time_used_2);
}

int main() {
    performance_test();
    return 0;
}