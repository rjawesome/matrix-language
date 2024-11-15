
#include "fraction.h"

Fraction mul_frac(Fraction a, Fraction b) {
    auto [whole, sq] = reduce_sqrt(a.sqrt * b.sqrt);
    int new_num = a.numerator * b.numerator * whole;
    int new_dem = a.denominator * b.denominator;
    int div = gcd(new_num, new_dem);
    return { new_num / div, new_dem / div, sq };
}

Fraction add_frac(Fraction a, Fraction b) {
    assert(a.sqrt == b.sqrt || a.numerator == 0 || b.numerator == 0);
    int common_denom = a.denominator * b.denominator / gcd(a.denominator, b.denominator);
    int new_num = a.numerator * (common_denom/a.denominator) + b.numerator * (common_denom/b.denominator);
    int div = gcd(new_num, common_denom);
    return {new_num / div, common_denom / div, a.numerator == 0 ? b.sqrt : a.sqrt };
}

Fraction negate_frac(Fraction a) {
    return { -a.numerator, a.denominator, a.sqrt };
}

Fraction inverse_frac(Fraction a) {
    int num = a.numerator;
    int dem = a.denominator;
    if (a.sqrt != 1) {
        num *= a.sqrt;
        int div = gcd(num, dem);
        num /= div;
        dem /= div;
    }
    if (a.numerator < 0) {
        return { -dem, -num, a.sqrt };
    }
    return { dem, num, a.sqrt };
}

Fraction sqrt_frac(Fraction a) {
    return mul_frac({1, 1, a.numerator}, inverse_frac({1, 1, a.denominator}));
}

void print_frac(Fraction const &m) {
    if (m.sqrt == 1 || m.numerator != 1) {
        cout << m.numerator;
    }
    if (m.sqrt != 1 && m.numerator != 0) {
        cout << "√" << m.sqrt;
    }
    if (m.denominator != 1 && m.numerator != 0) {
        cout << "/" << m.denominator;
    }
}

Error load_frac(string s, Fraction &m) {
    if (s.find("/") == string::npos) {
        m.denominator = 1;
        m.numerator = stoi(s);
    } else {
        m.numerator = stoi(s.substr(0, s.find("/")));
        m.denominator = stoi(s.substr(s.find("/") + 1));
    }
    expect(m.denominator != 0, "Cannot have a zero denominator");
    int div = gcd(m.numerator, m.denominator);
    m.numerator /= div;
    m.denominator /= div;
    m.sqrt = 1;
}