#ifndef FRACTION
#define FRACTION

#include <iostream>
#include "utils.h"
#include <immintrin.h>
#include <variant>
using namespace std;
typedef pair<pair<__m256i, __m256i>, __m256i> FracGroup;

struct Fraction {
    int numerator;
    int denominator; // should always be >= 1
    int sqrt; // multiples to numerator
};

struct Error; // defined in utils.h

#ifdef _WIN32
    const char SQRT_CH[2] = {(char)251, (char)0};
    const int SQRT_LEN = 1;
#else
    const char SQRT_CH[4] = "√";
    const int SQRT_LEN = 3;
#endif

void print_frac(Fraction const &m);
Error load_frac(string s, Fraction &m);
Fraction mul_frac(Fraction a, Fraction b);
variant<Error, pair<pair<__m256i, __m256i>, __m256i>> add_fracs(__m256i num1, __m256i den1, __m256i sqrt1, __m256i num2, __m256i den2, __m256i sqrt2);
variant<Error, Fraction> add_frac(Fraction a, Fraction b);
pair<pair<__m256i, __m256i>, __m256i> mul_fracs(__m256i num1, __m256i den1, __m256i sqrt1, __m256i num2, __m256i den2, __m256i sqrt2);
Fraction negate_frac(Fraction a);
Fraction inverse_frac(Fraction a);
Fraction sqrt_frac(Fraction a);

#endif