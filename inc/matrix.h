#ifndef MATRIX
#define MATRIX

#include "fraction.h"
#include "utils.h"
#include "vector.h"
#include <vector>
#include "assert.h"
using namespace std;

variant<string_view, vector<vector<Fraction>>> get_matrix();
void print_matrix(vector<vector<Fraction>> const &matrix);
vector<vector<Fraction>> transpose(vector<vector<Fraction>> const &matrix);
variant<string_view, vector<vector<Fraction>>> add_matrix(vector<vector<Fraction>> const &matrix1, vector<vector<Fraction>> const &matrix2);
vector<vector<Fraction>> matrix_scale(Fraction f, vector<vector<Fraction>> const &matrix1);
variant<string_view, vector<vector<Fraction>>> orthonormalize_rows(vector<vector<Fraction>> const &matrix);
variant<string_view, vector<vector<Fraction>>> mat_mul(vector<vector<Fraction>> const &matrix1, vector<vector<Fraction>> const &matrix2);
vector<vector<Fraction>> rref(const vector<vector<Fraction>> &matrix);

#endif