#include <bits/stdc++.h>
#include "utils.h"
using namespace std;

void print_matrix(vector<vector<Fraction>> const &matrix) {
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

void print_vector(vector<Fraction> const &vector, string prefix) {
    cout << prefix << endl;
    cout << DASH << endl;
    for (Fraction i : vector) {
        cout << i << " ";
    }
    cout << endl;
    cout << DASH << endl;
}

int gcd(int a, int b) {
    a = abs(a);
    b = abs(b);
    if (a < b) {
        int temp = a;
        a = b;
        b = temp;
    }
    while (b != 0) {
        int nb = a % b;
        a = b;
        b = nb;
    }
    return a;
}

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

void rref(vector<vector<Fraction>> &matrix) {
    assert(matrix.size() > 0);
    int rows = matrix.size();
    int cols = matrix[0].size();
    // ref
    for (int i = 0; i < min(rows, cols); i++) {
        if (matrix[i][i].numerator == 0) {
            // see if we need to do a swap
            for (int j = i + 1; j < rows; j++) {
                if (matrix[j][i].numerator != 0) {
                    // swap
                    for (int k = 0; k < cols; k++) {
                        Fraction temp = matrix[i][k];
                        matrix[i][k] = matrix[j][k];
                        matrix[j][k] = temp;
                    }
                    break;
                }
            }
        }
        for (int j = i + 1; j < rows; j++) {
            if (matrix[j][i].numerator != 0) {
                Fraction mul = negate_frac(mul_frac(matrix[j][i], inverse_frac(matrix[i][i])));
                for (int k = 0; k < cols; k++) {
                    matrix[j][k] = add_frac(matrix[j][k], mul_frac(matrix[i][k], mul));
                }
            }
        }
    }

    // reverse ref
    for (int i = min(rows,cols)-1; i >= 0; i--) {
        if (matrix[i][i].numerator == 0) {
            // see if we need to do a swap
            for (int j = i - 1; j >= 0; j--) {
                if (matrix[j][i].numerator != 0) {
                    // swap
                    for (int k = 0; k < cols; k++) {
                        Fraction temp = matrix[i][k];
                        matrix[i][k] = matrix[j][k];
                        matrix[j][k] = temp;
                    }
                    break;
                }
            }
        }

        // normalize this row (if necessary)
        if (matrix[i][i].numerator != 0) {
            Fraction mul = inverse_frac(matrix[i][i]);
            for (int k = 0; k < cols; k++) {
                matrix[i][k] = mul_frac(matrix[i][k], mul);
            }
        }

        for (int j = i - 1; j >= 0; j--) {
            if (matrix[j][i].numerator != 0) {
                Fraction mul = negate_frac(mul_frac(matrix[j][i], inverse_frac(matrix[i][i])));
                for (int k = 0; k < cols; k++) {
                    matrix[j][k] = add_frac(matrix[j][k], mul_frac(matrix[i][k], mul));
                }
            }
        }
    }
}

Fraction dot(vector<Fraction> const &vector1, vector<Fraction> const &vector2) {
    Fraction sum;
    for (int i = 0; i < min(vector1.size(), vector2.size()); i++) {
        sum = add_frac(sum, mul_frac(vector1[i], vector2[i]));
    }
    return sum;
}

vector<vector<Fraction>> transpose(vector<vector<Fraction>> const &matrix) {
    assert(matrix.size() > 0);
    vector<vector<Fraction>> t(matrix[0].size(), vector<Fraction>(matrix.size()));
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[0].size(); j++) {
            t[j][i] = matrix[i][j];
        }
    }
    return t;
}

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

vector<vector<Fraction>> orthonormalize_rows(vector<vector<Fraction>> const &matrix) {
    assert(matrix.size() > 0);
    vector<vector<Fraction>> output(matrix.size());
    for (int i = 0; i < matrix.size(); i++) {
        output[i] = matrix[i];
        for (int j = i-1; j >= 0; j--) {
            vector<Fraction> proj = mul_vector(dot(matrix[i], output[j]), output[j]);
            output[i] = sub_vectors(output[i], proj);
        }
        output[i] = normalize(output[i]);
    }
    return output;
}


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
    return { a.numerator * (common_denom/a.denominator) + b.numerator * (common_denom/b.denominator), common_denom, a.numerator == 0 ? b.sqrt : a.sqrt };
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

ostream &operator<<(ostream &os, Fraction const &m) {
    os << m.numerator;
    if (m.sqrt != 1) {
        os << "√" << m.sqrt;
    }
    if (m.denominator != 1) {
        os << "/" << m.denominator;
    }
    return os;
}

istream &operator>>(istream &is, Fraction &m) {
    string s; is >> s;
    if (s.find("/") == string::npos) {
        m.denominator = 1;
        m.numerator = stoi(s);
    } else {
        m.numerator = stoi(s.substr(0, s.find("/")));
        m.denominator = stoi(s.substr(s.find("/") + 1));
    }
    int div = gcd(m.numerator, m.denominator);
    m.numerator /= div;
    m.denominator /= div;
    return is;
}