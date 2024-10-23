#ifndef VECTOR
#define VECTOR

#include "fraction.h"
#include "utils.h"
#include <vector>
using namespace std;

Fraction dot(vector<Fraction> const &a, vector<Fraction> const &b);
vector<Fraction> normalize(vector<Fraction> const &a);
vector<Fraction> add_vectors(vector<Fraction> const &a, vector<Fraction> const &b);
vector<Fraction> sub_vectors(vector<Fraction> const &a, vector<Fraction> const &b);
vector<Fraction> mul_vector(Fraction a, vector<Fraction> const &b);

void print_vector(vector<Fraction> const &vector);

#endif