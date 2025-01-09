#include "matrix.h"

variant<Error, vector<vector<Fraction>>> rref(const vector<vector<Fraction>> &original) {
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
                    unwrap(Fraction, matrix[j][k], add_frac(matrix[j][k], mul_frac(matrix[i][k], mul)));
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
                    unwrap(Fraction, matrix[j][k], add_frac(matrix[j][k], mul_frac(matrix[i][k], mul)));
                }
            }
        }
    }
    return matrix;
}

// similar to rref algorithm
variant<Error, Fraction> determinant(const vector<vector<Fraction>> &original) {
    if (original.size() == 0) {
        return Fraction{0, 1, 1};
    }
    expect(original.size() == original[0].size(), "Cannot take determinant of non-square matrix");
    Fraction determ = {1, 1, 1};
    vector<vector<Fraction>> matrix = original;
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
                    determ = negate_frac(determ);
                    break;
                }
            }
        }
        for (int j = i + 1; j < rows; j++) {
            if (matrix[j][i].numerator != 0) {
                Fraction mul = negate_frac(mul_frac(matrix[j][i], inverse_frac(matrix[i][i])));
                for (int k = 0; k < cols; k++) {
                    unwrap(Fraction, matrix[j][k], add_frac(matrix[j][k], mul_frac(matrix[i][k], mul)));
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
                    determ = negate_frac(determ);
                    break;
                }
            }
        }

        // normalize this row (if necessary)
        if (matrix[i][i].numerator != 0) {
            Fraction mul = inverse_frac(matrix[i][i]);
            determ = mul_frac(determ, matrix[i][i]);
            for (int k = 0; k < cols; k++) {
                matrix[i][k] = mul_frac(matrix[i][k], mul);
            }
        }

        for (int j = i - 1; j >= 0; j--) {
            if (matrix[j][i].numerator != 0) {
                Fraction mul = negate_frac(mul_frac(matrix[j][i], inverse_frac(matrix[i][i])));
                for (int k = 0; k < cols; k++) {
                    unwrap(Fraction, matrix[j][k], add_frac(matrix[j][k], mul_frac(matrix[i][k], mul)));
                }
            }
        }
    }
    // check that the matrix is identity
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix.size(); j++) {
            if (matrix[i][j].numerator != 0 && i != j) return Fraction{0, 1, 1};
            if (matrix[i][j].numerator == 0 && i == j) return Fraction{0, 1, 1};
        }
    }
    return determ;
}


// similar to rref algorithm
variant<Error, vector<vector<Fraction>>> inverse(const vector<vector<Fraction>> &original) {
    vector<vector<Fraction>> matrix = original;
    if (matrix.size() == 0) {
        return matrix;
    }
    expect(matrix.size() == matrix[0].size(), "Cannot take inverse of non-square matrix");
    int rows = matrix.size();
    int cols = matrix[0].size();
    vector<vector<Fraction>> inverse = vector<vector<Fraction>>(rows, vector<Fraction>(cols));
    // gen identity
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < rows; j++) {
            if (i == j) inverse[i][j] = {1, 1, 1};
            else inverse[i][j] = {0, 1, 1};
        }
    }
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

                        Fraction temp2 = inverse[i][k];
                        inverse[i][k] = inverse[j][k];
                        inverse[j][k] = temp;
                    }
                    break;
                }
            }
        }
        for (int j = i + 1; j < rows; j++) {
            if (matrix[j][i].numerator != 0) {
                Fraction mul = negate_frac(mul_frac(matrix[j][i], inverse_frac(matrix[i][i])));
                for (int k = 0; k < cols; k++) {
                    unwrap(Fraction, matrix[j][k], add_frac(matrix[j][k], mul_frac(matrix[i][k], mul)));
                    unwrap(Fraction, inverse[j][k], add_frac(inverse[j][k], mul_frac(inverse[i][k], mul)));
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

                        Fraction temp2 = inverse[i][k];
                        inverse[i][k] = inverse[j][k];
                        inverse[j][k] = temp;
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
                inverse[i][k] = mul_frac(inverse[i][k], mul);
            }
        }

        for (int j = i - 1; j >= 0; j--) {
            if (matrix[j][i].numerator != 0) {
                Fraction mul = negate_frac(mul_frac(matrix[j][i], inverse_frac(matrix[i][i])));
                for (int k = 0; k < cols; k++) {
                    unwrap(Fraction, matrix[j][k], add_frac(matrix[j][k], mul_frac(matrix[i][k], mul)));
                    unwrap(Fraction, inverse[j][k], add_frac(inverse[j][k], mul_frac(inverse[i][k], mul)));
                }
            }
        }
    }
    // check that the matrix is identity
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix.size(); j++) {
            expect(
                (
                    (i == j && matrix[i][j].numerator == 1 && matrix[i][j].denominator == 1) ||
                    (i != j && matrix[i][j].numerator == 0)
                ),
                "Matrix has non-linearly independent columns"
            );
        }
    }
    return inverse;
}

Fraction norm_sq(vector<vector<Fraction>> const &matrix) {
    Fraction sum = {0, 1, 1};
    for (int i = 0; i < matrix.size(); i++) {
        // dot product removes any square roots
        sum = get<Fraction>(add_frac(sum, get<Fraction>(dot(matrix[i], matrix[i]))));
    }
    return sum;
}

variant<Error, vector<vector<Fraction>>> orthonormalize_rows(vector<vector<Fraction>> const &matrix) {
    if (matrix.size() == 0) {
        return vector<vector<Fraction>>();
    }
    vector<vector<Fraction>> output(matrix.size());
    for (int i = 0; i < matrix.size(); i++) {
        output[i] = matrix[i];
        for (int j = i-1; j >= 0; j--) {
            Fraction dotp;
            unwrap(Fraction, dotp, dot(matrix[i], output[j]));
            vector<Fraction> proj = mul_vector(dotp, output[j]);
            unwrap(vector<Fraction>, output[i], sub_vectors(output[i], proj));
        }
        expect(get<Fraction>(dot(output[i], output[i])).numerator != 0, "The vectors to be orthonormalized are not linearly independent");
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

variant<Error, vector<vector<Fraction>>> mat_mul(vector<vector<Fraction>> const &matrix1, vector<vector<Fraction>> const &matrix2) {
    expect(matrix1.size() > 0 && matrix2.size() > 0 && matrix1[0].size() == matrix2.size(), "Matrix multiplication dimensions invalid");
    vector<vector<Fraction>> output(matrix1.size(), vector<Fraction>(matrix2[0].size()));
    for (int i = 0; i < matrix1.size(); i++) {
        for (int j = 0; j < matrix2[0].size(); j++) {
            Fraction sum = {0, 1, 1};
            for (int k = 0; k < matrix2.size(); k++) {
                unwrap(Fraction, sum, add_frac(sum, mul_frac(matrix1[i][k], matrix2[k][j])));
            }
            output[i][j] = sum;
        }
    }
    return output;
}

variant<Error, vector<vector<Fraction>>> mat_mul_simd(vector<vector<Fraction>> const &matrix1, vector<vector<Fraction>> const &matrix2) {
    expect(matrix1.size() > 0 && matrix2.size() > 0 && matrix1[0].size() == matrix2.size(), "Matrix multiplication dimensions invalid");
    if (matrix2[0].size() < matrix1.size()) {
        return mat_mul_simd(matrix2, matrix1);
    }
    vector<vector<Fraction>> output(matrix1.size(), vector<Fraction>(matrix2[0].size()));
    for (int i = 0; i < matrix1.size(); i++) {
        for (int j = 0; j < matrix2[0].size(); j+=8) {
            if (j + 8 > matrix2[0].size()) {
                while (j < matrix2[0].size()) {
                    Fraction sum = {0, 1, 1};
                    for (int k = 0; k < matrix2.size(); k++) {
                        unwrap(Fraction, sum, add_frac(sum, mul_frac(matrix1[i][k], matrix2[k][j])));
                    }
                    output[i][j] = sum;
                    j++;
                }
            }
            else {
                FracGroup sums = {{_mm256_set1_epi32(0), _mm256_set1_epi32(1)}, _mm256_set1_epi32(1)};
                for (int k = 0; k < matrix2.size(); k++) {
                    int num1[8];
                    int den1[8];
                    int sqrt1[8];
                    int num2[8];
                    int den2[8];
                    int sqrt2[8];
                    for (int l = 0; l < 8; l++) {
                        num1[l] = matrix1[i][k].numerator;
                        den1[l] = matrix1[i][k].denominator;
                        sqrt1[l] = matrix1[i][k].sqrt;
                        num2[l] = matrix2[k][j+l].numerator;
                        den2[l] = matrix2[k][j+l].denominator;
                        sqrt2[l] = matrix2[k][j+l].sqrt;
                    }
                    cout<<"a"<< endl;
                    FracGroup muls = mul_fracs(_mm256_load_si256((__m256i*)num1), _mm256_load_si256((__m256i*)den1), _mm256_load_si256((__m256i*)sqrt1), _mm256_load_si256((__m256i*)num2), _mm256_load_si256((__m256i*)den2), _mm256_load_si256((__m256i*)sqrt2));
                    cout<<"b"<< endl;
                    print_m256(muls.first.first);
                    print_m256(muls.first.second);
                    print_m256(muls.second);
                    unwrap(FracGroup, sums, add_fracs(sums.first.first, sums.first.second, sums.second, muls.first.first, muls.first.second, muls.second));
                }
                int nums[8];
                int dens[8];
                int sqrts[8];
                _mm256_store_si256((__m256i*)nums, sums.first.first);
                _mm256_store_si256((__m256i*)dens, sums.first.second);
                _mm256_store_si256((__m256i*)sqrts, sums.second);
                for (int l = 0; l < 8; l++) {
                    output[i][j+l].numerator = nums[l];
                    output[i][j+l].denominator = dens[l];
                    output[i][j+l].sqrt = sqrts[l];
                }
            }
        }
    }
    return output;
}

variant<Error, vector<vector<Fraction>>> add_matrix(vector<vector<Fraction>> const &matrix1, vector<vector<Fraction>> const &matrix2) {
    expect(matrix1.size() == matrix2.size() && (matrix1.size() == 0 || matrix1[0].size() == matrix2[0].size()), "Cannot add matrices");
    int rows = matrix1.size();
    int cols = matrix1.size() > 0 ? matrix1[0].size() : 0;
    vector<vector<Fraction>> matrix(rows, vector<Fraction>(cols));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            unwrap(Fraction, matrix[i][j], add_frac(matrix1[i][j], matrix2[i][j]));
        }
    }
    return matrix;
}
vector<vector<Fraction>> matrix_scale(Fraction f, vector<vector<Fraction>> const &matrix1) {
    vector<vector<Fraction>> matrix(matrix1.size(), vector<Fraction>(matrix1.size() > 0 ? matrix1[0].size() : 0));
    for (int i = 0; i < matrix1.size(); i++) {
        for (int j = 0; j < matrix1[i].size(); j++) {
            matrix[i][j] = mul_frac(f, matrix1[i][j]);
        }
    }
    return matrix;
}

variant<Error, vector<vector<Fraction>>> get_matrix(int rows, int cols) {
    expect(rows > 0 && cols > 0, "Matrix needs positive rows and columns");
    vector<vector<Fraction>> matrix(rows, vector<Fraction>(cols));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            string s; cin >> s;
            load_frac(s, matrix[i][j]);
        }
    }
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