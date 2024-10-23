#ifndef MATRIX
#define MATRIX

#include "fraction.h"
#include "utils.h"
#include "vector.h"
#include <vector>
#include "assert.h"
using namespace std;

void print_matrix(vector<vector<Fraction>> const &matrix);
vector<vector<Fraction>> transpose(vector<vector<Fraction>> const &matrix);
vector<vector<Fraction>> orthonormalize_rows(vector<vector<Fraction>> const &matrix);
void rref(vector<vector<Fraction>> &matrix);

#endif