#ifndef MATRIX
#define MATRIX

#include "fraction.h"
#include "utils.h"
#include "vector.h"
#include <vector>
#include "assert.h"
using namespace std;

variant<Error, vector<vector<Fraction>>> get_matrix();
void print_matrix(vector<vector<Fraction>> const &matrix);
vector<vector<Fraction>> transpose(vector<vector<Fraction>> const &matrix);
variant<Error, vector<vector<Fraction>>> add_matrix(vector<vector<Fraction>> const &matrix1, vector<vector<Fraction>> const &matrix2);
vector<vector<Fraction>> matrix_scale(Fraction f, vector<vector<Fraction>> const &matrix1);
variant<Error, vector<vector<Fraction>>> orthonormalize_rows(vector<vector<Fraction>> const &matrix);
variant<Error, vector<vector<Fraction>>> mat_mul(vector<vector<Fraction>> const &matrix1, vector<vector<Fraction>> const &matrix2);
vector<vector<Fraction>> rref(const vector<vector<Fraction>> &matrix);
variant<Error, vector<vector<Fraction>>> inverse(const vector<vector<Fraction>> &matrix);
variant<Error, Fraction> determinant(const vector<vector<Fraction>> &matrix);
Fraction norm_sq(const vector<vector<Fraction>> &matrix);

#endif