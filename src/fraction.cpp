
#include "fraction.h"

Fraction mul_frac(Fraction a, Fraction b) {
    auto [whole, sq] = reduce_sqrts(a.sqrt, b.sqrt);
    int new_num = a.numerator * b.numerator * whole;
    int new_dem = a.denominator * b.denominator;
    int div = gcd(new_num, new_dem);
    return { new_num / div, new_dem / div, sq };
}
pair<pair<__m256i, __m256i>, __m256i> mul_fracs(__m256i num1, __m256i den1, __m256i sqrt1, __m256i num2, __m256i den2, __m256i sqrt2) {
    auto [whole, sq] = reduce_sqrts(sqrt1, sqrt2);
    __m256i new_num = _mm256_mullo_epi32(_mm256_mullo_epi32(num1, num2), whole);
    __m256i new_dem = _mm256_mullo_epi32(den1, den2);
    __m256i div = gcd(new_num, new_dem);
    return { {divide_m256i_exact(new_num, div), divide_m256i_exact(new_dem, div)}, sq };
}

variant<Error, Fraction> add_frac(Fraction a, Fraction b) {
    expect(a.sqrt == b.sqrt || a.numerator == 0 || b.numerator == 0, "Addition between fractions with different square roots is not supported");
    int common_denom = a.denominator * (b.denominator / gcd(a.denominator, b.denominator));
    int new_num = a.numerator * (common_denom/a.denominator) + b.numerator * (common_denom/b.denominator);
    int div = gcd(new_num, common_denom);
    return Fraction{new_num / div, common_denom / div, a.numerator == 0 ? b.sqrt : a.sqrt };
}

variant<Error, pair<pair<__m256i, __m256i>, __m256i>> add_fracs(__m256i num1, __m256i den1, __m256i sqrt1, __m256i num2, __m256i den2, __m256i sqrt2) {
    __m256i test = _mm256_andnot_si256(_mm256_cmpeq_epi32(sqrt1, sqrt2), _mm256_and_si256(_mm256_cmpgt_epi32(num1, _mm256_setzero_si256()), _mm256_cmpgt_epi32(num2, _mm256_setzero_si256())));
    expect(_mm256_testz_si256(test, test), "Addition between fractions with different square roots is not supported");
    __m256i common_denom = _mm256_mullo_epi32(den1, divide_m256i_exact(den2, gcd(den1, den2)));
    __m256i new_num = _mm256_add_epi32(_mm256_mullo_epi32(num1, divide_m256i_exact(common_denom, den1)), _mm256_mullo_epi32(num2, divide_m256i_exact(common_denom, den2)));
    __m256i div = gcd(new_num, common_denom);
    return pair<pair<__m256i, __m256i>, __m256i>{ { divide_m256i_exact(new_num, div), divide_m256i_exact(common_denom, div) }, _mm256_max_epi32(sqrt1, sqrt2) };
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
        cout << SQRT_CH << m.sqrt;
    }
    if (m.denominator != 1 && m.numerator != 0) {
        cout << "/" << m.denominator;
    }
}

Error load_frac(string s, Fraction &m) {
    if (s.find("/") == string::npos) {
        m.denominator = 1;
        if (s.find(SQRT_CH) != string::npos) {
            m.sqrt = stoi(s.substr(s.find(SQRT_CH) + SQRT_LEN));
            string num = s.substr(0, s.find(SQRT_CH));
            if (num.length() == 0) m.numerator = 1;
            else if (num.length() == 1 && num[0] == '-') m.numerator = -1;
            else m.numerator = stoi(num);
        } else {
            m.sqrt = 1;
            m.numerator = stoi(s);
        }
    } else {
        string top = s.substr(0, s.find("/"));
        if (top.find(SQRT_CH) != string::npos) {
            m.sqrt = stoi(top.substr(top.find(SQRT_CH) + SQRT_LEN));
            string num = top.substr(0, top.find(SQRT_CH));
            if (num.length() == 0) m.numerator = 1;
            else if (num.length() == 1 && num[0] == '-') m.numerator = -1;
            else m.numerator = stoi(num);
        } else {
            m.sqrt = 1;
            m.numerator = stoi(top);
        }
        m.denominator = stoi(s.substr(s.find("/") + 1));
    }
    expect(m.denominator != 0, "Cannot have a zero denominator");
    auto [whole, sq] = reduce_sqrt(m.sqrt);
    m.sqrt = sq;
    m.numerator *= whole;
    int div = gcd(m.numerator, m.denominator);
    m.numerator /= div;
    m.denominator /= div;
    return {"", 0};
}