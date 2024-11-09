#include "matrix.h"

vector<vector<Fraction>> rref(const vector<vector<Fraction>> &original) {
    vector<vector<Fraction>> matrix = original;
    if (matrix.size() == 0) {
        return vector<vector<Fraction>>();
    }
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
    return matrix;
}

variant<string_view, vector<vector<Fraction>>> orthonormalize_rows(vector<vector<Fraction>> const &matrix) {
    if (matrix.size() == 0) {
        return vector<vector<Fraction>>();
    }
    vector<vector<Fraction>> output(matrix.size());
    for (int i = 0; i < matrix.size(); i++) {
        output[i] = matrix[i];
        for (int j = i-1; j >= 0; j--) {
            vector<Fraction> proj = mul_vector(dot(matrix[i], output[j]), output[j]);
            output[i] = sub_vectors(output[i], proj);
        }
        if (dot(output[i], output[i]).numerator == 0) {
            return "The vectors to be orthonormalized are not linearly independent";
        }
        output[i] = normalize(output[i]);
    }
    return output;
}

vector<vector<Fraction>> transpose(vector<vector<Fraction>> const &matrix) {
    if (matrix.size() == 0) {
        return vector<vector<Fraction>>();
    }
    vector<vector<Fraction>> t(matrix[0].size(), vector<Fraction>(matrix.size()));
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[0].size(); j++) {
            t[j][i] = matrix[i][j];
        }
    }
    return t;
}

variant<string_view, vector<vector<Fraction>>> mat_mul(vector<vector<Fraction>> const &matrix1, vector<vector<Fraction>> const &matrix2) {
    expect(matrix1.size() > 0 && matrix2.size() > 0 && matrix1[0].size() == matrix2.size(), "Matrix multiplication dimensions invalid");
    vector<vector<Fraction>> output(matrix1.size(), vector<Fraction>(matrix2[0].size()));
    for (int i = 0; i < matrix1.size(); i++) {
        for (int j = 0; j < matrix2[0].size(); j++) {
            Fraction sum = {0, 1, 1};
            for (int k = 0; k < matrix2.size(); k++) {
                sum = add_frac(sum, mul_frac(matrix1[i][k], matrix2[k][j]));
            }
            output[i][j] = sum;
        }
    }
    return output;
}

variant<string_view, vector<vector<Fraction>>> get_matrix() {
    int rows, cols; cin >> rows >> cols;
    expect(rows > 0 && cols > 0, "Matrix needs positive rows and columns");
    vector<vector<Fraction>> matrix(rows, vector<Fraction>(cols));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            string s; cin >> s;
            load_frac(s, matrix[i][j]);
        }
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear cin
    return matrix;
}

void print_matrix(vector<vector<Fraction>> const &matrix) {
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            print_frac(matrix[i][j]);
            cout << " ";
        }
        cout << endl;
    }
}