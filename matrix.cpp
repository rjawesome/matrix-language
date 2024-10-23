#include "matrix.h"

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

void print_matrix(vector<vector<Fraction>> const &matrix) {
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}