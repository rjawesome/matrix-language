#include <bits/stdc++.h>
using namespace std;

const string DASH = "-------------";
const char VAR_NAMES[] = {'x','y','z','w','a','b','c'};
const int VAR_NAME_COUNT = sizeof(VAR_NAMES)/sizeof(VAR_NAMES[0]);

struct Fraction {
    int numerator = 0;
    int denominator = 1; // should always be >= 1
};

void rref(vector<vector<Fraction>>& matrix, int rows, int cols);
int gcd(int a, int b);
void print_matrix(vector<vector<Fraction>>& matrix, int rows, int cols);
void print_vector(vector<Fraction>& vector, string prefix);

ostream &operator<<(ostream &os, Fraction const &m);
istream &operator>>(istream &is, Fraction &m);
Fraction mul_frac(Fraction a, Fraction b);
Fraction add_frac(Fraction a, Fraction b);
Fraction negate_frac(Fraction a);
Fraction inverse_frac(Fraction a);