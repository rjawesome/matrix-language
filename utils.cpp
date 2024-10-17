#include <bits/stdc++.h>
#include "utils.h"
using namespace std;

void rref(vector<vector<double>>& matrix, int rows, int cols) {
    // ref
    for (int i = 0; i < min(rows, cols); i++) {
        if (matrix[i][i] == 0) {
            // see if we need to do a swap
            for (int j = i + 1; j < rows; j++) {
                if (matrix[j][i] != 0) {
                    // swap
                    for (int k = 0; k < cols; k++) {
                        double temp = matrix[i][k];
                        matrix[i][k] = matrix[j][k];
                        matrix[j][k] = temp;
                    }
                    break;
                }
            }
        }
        for (int j = i + 1; j < rows; j++) {
            if (matrix[j][i] != 0) {
                double mul = -(matrix[j][i]/matrix[i][i]);
                for (int k = 0; k < cols; k++) {
                    matrix[j][k] += matrix[i][k] * mul;
                }
            }
        }
    }

    // reverse ref
    for (int i = min(rows,cols)-1; i >= 0; i--) {
        if (matrix[i][i] == 0) {
            // see if we need to do a swap
            for (int j = i - 1; j >= 0; j--) {
                if (matrix[j][i] != 0) {
                    // swap
                    for (int k = 0; k < cols; k++) {
                        double temp = matrix[i][k];
                        matrix[i][k] = matrix[j][k];
                        matrix[j][k] = temp;
                    }
                    break;
                }
            }
        }

        // normalize this row (if necessary)
        if (matrix[i][i] != 0) {
            double div = matrix[i][i];
            for (int k = 0; k < cols; k++) {
                matrix[i][k] /= div;
            }
        }

        for (int j = i - 1; j >= 0; j--) {
            if (matrix[j][i] != 0) {
                double mul = -(matrix[j][i]/matrix[i][i]);
                for (int k = 0; k < cols; k++) {
                    matrix[j][k] += matrix[i][k] * mul;
                }
            }
        }
    }
}

void print_matrix(vector<vector<double>>& matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

void print_vector(vector<double>& vector, string prefix) {
    cout << prefix << endl;
    cout << DASH << endl;
    for (int i : vector) {
        cout << i << " ";
    }
    cout << endl;
    cout << DASH << endl;
}