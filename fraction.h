#ifndef FRACTION
#define FRACTION

#include <iostream>
#include <cassert>
#include "utils.h"
using namespace std;

struct Fraction {
    int numerator = 0;
    int denominator = 1; // should always be >= 1
    int sqrt = 1; // multiples to numerator
};

ostream &operator<<(ostream &os, Fraction const &m);
istream &operator>>(istream &is, Fraction &m);
Fraction mul_frac(Fraction a, Fraction b);
Fraction add_frac(Fraction a, Fraction b);
Fraction negate_frac(Fraction a);
Fraction inverse_frac(Fraction a);
Fraction sqrt_frac(Fraction a);

#endif