#include <bits/stdc++.h>
using namespace std;

const string DASH = "-------------";
const char VAR_NAMES[] = {'x','y','z','w','a','b','c'};
const int VAR_NAME_COUNT = sizeof(VAR_NAMES)/sizeof(VAR_NAMES[0]);

struct Fraction {
    int numerator = 0;
    int denominator = 1; // should always be >= 1
    int sqrt = 1; // multiples to numerator
};

void print_matrix(vector<vector<Fraction>> const &matrix);
void print_vector(vector<Fraction> const &vector, string prefix);

int gcd(int a, int b);

Fraction dot(vector<Fraction> const &a, vector<Fraction> const &b);
vector<Fraction> normalize(vector<Fraction> const &a);
vector<Fraction> add_vectors(vector<Fraction> const &a, vector<Fraction> const &b);
vector<Fraction> sub_vectors(vector<Fraction> const &a, vector<Fraction> const &b);
vector<vector<Fraction>> transpose(vector<vector<Fraction>> const &matrix);

vector<vector<Fraction>> orthonormalize_rows(vector<vector<Fraction>> const &matrix);
void rref(vector<vector<Fraction>> &matrix);

ostream &operator<<(ostream &os, Fraction const &m);
istream &operator>>(istream &is, Fraction &m);
Fraction mul_frac(Fraction a, Fraction b);
Fraction add_frac(Fraction a, Fraction b);
Fraction negate_frac(Fraction a);
Fraction inverse_frac(Fraction a);
Fraction sqrt_frac(Fraction a);