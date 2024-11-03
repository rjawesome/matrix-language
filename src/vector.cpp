#include "vector.h"

vector<Fraction> normalize(vector<Fraction> const &a) {
    Fraction inv_norm = inverse_frac(sqrt_frac(dot(a, a)));
    vector<Fraction> out(a.size());
    for (int i = 0; i < a.size(); i++) {
        out[i] = mul_frac(inv_norm, a[i]);
    }
    return out;
}

vector<Fraction> add_vectors(vector<Fraction> const &a, vector<Fraction> const &b) {
    vector<Fraction> output(min(a.size(), b.size()));
    for (int i = 0; i < min(a.size(), b.size()); i++) {
        output[i] = add_frac(a[i], b[i]);
    }
    return output;
}

vector<Fraction> sub_vectors(vector<Fraction> const &a, vector<Fraction> const &b) {
    vector<Fraction> output(min(a.size(), b.size()));
    for (int i = 0; i < min(a.size(), b.size()); i++) {
        output[i] = add_frac(a[i], negate_frac(b[i]));
    }
    return output;
}

vector<Fraction> mul_vector(Fraction a, vector<Fraction> const &b) {
    vector<Fraction> output(b.size());
    for (int i = 0; i < b.size(); i++) {
        output[i] = mul_frac(a, b[i]);
    }
    return output;
}


Fraction dot(vector<Fraction> const &vector1, vector<Fraction> const &vector2) {
    Fraction sum = {0, 1, 1};
    for (int i = 0; i < min(vector1.size(), vector2.size()); i++) {
        sum = add_frac(sum, mul_frac(vector1[i], vector2[i]));
    }
    return sum;
}

void print_vector(vector<Fraction> const &vector) {
    cout << DASH << endl;
    for (Fraction i : vector) {
        print_frac(i);
    }
    cout << endl;
    cout << DASH << endl;
}
