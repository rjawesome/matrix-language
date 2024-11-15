#ifndef FRACTION
#define FRACTION

#include <iostream>
#include "utils.h"
#include <cassert>
using namespace std;

struct Fraction {
    int numerator;
    int denominator; // should always be >= 1
    int sqrt; // multiples to numerator
};

void print_frac(Fraction const &m);
Error load_frac(string s, Fraction &m);
Fraction mul_frac(Fraction a, Fraction b);
Fraction add_frac(Fraction a, Fraction b);
Fraction negate_frac(Fraction a);
Fraction inverse_frac(Fraction a);
Fraction sqrt_frac(Fraction a);

#endif