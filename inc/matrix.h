#ifndef MATRIX
#define MATRIX

#include "fraction.h"
#include "utils.h"
#include "vector.h"
#include <vector>
#include "assert.h"
#include "parser.h"
using namespace std;

vector<vector<Fraction>> get_matrix();
void print_matrix(vector<vector<Fraction>> const &matrix);
vector<vector<Fraction>> transpose(vector<vector<Fraction>> const &matrix);
vector<vector<Fraction>> orthonormalize_rows(vector<vector<Fraction>> const &matrix);
vector<vector<Fraction>> mat_mul(vector<vector<Fraction>> const &matrix1, vector<vector<Fraction>> const &matrix2);
vector<vector<Fraction>> rref(const vector<vector<Fraction>> &matrix);

#endif