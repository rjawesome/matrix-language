#include <bits/stdc++.h>
#include "utils.h"
using namespace std;

void rref(vector<vector<Fraction>>& matrix, int rows, int cols) {
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

void print_matrix(vector<vector<Fraction>>& matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

void print_vector(vector<Fraction>& vector, string prefix) {
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

Fraction mul_frac(Fraction a, Fraction b) {
    int new_num = a.numerator * b.numerator;
    int new_dem = a.denominator * b.denominator;
    int div = gcd(new_num, new_dem);
    return { new_num / div, new_dem / div };
}

Fraction add_frac(Fraction a, Fraction b) {
    int common_denom = a.denominator * b.denominator / gcd(a.denominator, b.denominator);
    return { a.numerator * (common_denom/a.denominator) + b.numerator * (common_denom/b.denominator), common_denom};
}

Fraction negate_frac(Fraction a) {
    return { -a.numerator, a.denominator };
}

Fraction inverse_frac(Fraction a) {
    if (a.numerator < 0) {
        return { -a.denominator, -a.numerator };
    }
    return { a.denominator, a.numerator };
}

ostream &operator<<(ostream &os, Fraction const &m) {
    if (m.denominator == 1) {
        os << m.numerator;
    } else {
        os << m.numerator << "/" << m.denominator;
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