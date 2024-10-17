#include <bits/stdc++.h>

using namespace std;

const string DASH = "-------------";
const char VAR_NAMES[] = {'x','y','z','w','a','b','c'};
const int VAR_NAME_COUNT = sizeof(VAR_NAMES)/sizeof(VAR_NAMES[0]);

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

void no_solutions() {
    cout << "No solutions!" << endl;
}

void one_solution(vector<vector<double>>& matrix, int variables) {
    cout << "One solution!" << endl;
    cout << DASH << endl;
    for (int i = 0; i < variables; i++) {
        // vector the end of the matrix
        cout << matrix[i][variables] << " ";
    }
    cout << endl;
    cout << DASH << endl;
}

void multiple_solutions(vector<vector<double>>& matrix, int variables, int rows) {
    // free var col # => vector
    map<int, vector<double>> solutions;
    vector<double> offset(variables);
    for (int i = 0; i < variables; i++) {
        if (i >= rows || matrix[i][i] == 0) {
            vector<double> solution(variables);
            solution[i] = 1; // each free variable contains itself
            solutions[i] = solution;
        }
    }
    for (int i = 0; i < rows; i++) {
        // find the determined variable
        if (matrix[i][i] == 0) continue;
        // update each free var sol if needed
        for (int j = 0; j < variables; j++) {
            if (matrix[i][j] != 0 && solutions.find(j) != solutions.end()) {
                solutions[j][i] = -matrix[i][j]/matrix[i][i];
            }
        }

        // update the offset (end of equation / coefficient)
        offset[i] = matrix[i][variables]/matrix[i][i];
    }

    cout << "Multiple solutions!" << endl;
    int cnt = 0;
    for (auto [_, solution] : solutions) {
        cout << DASH << endl;
        cout << VAR_NAMES[cnt%VAR_NAME_COUNT];
        if (cnt>=VAR_NAME_COUNT) cout << cnt/VAR_NAME_COUNT;
        cout << endl;

        for (double i : solution) {
            // vector the end of the matrix
            cout << i << " ";
        }
        cout << endl;
        cout << DASH << endl;
        cnt++;
    }
    cout << "Offset" << endl;
    for (double i : offset) {
        // vector the end of the matrix
        cout << i << " ";
    }
    cout << endl;
    cout << DASH << endl;
}

void print_matrix(vector<vector<double>>& matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

int main () {
    int rows, cols; cin >> rows >> cols;
    vector<vector<double>> matrix(rows, vector<double>(cols));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cin >> matrix[i][j];
        }
    }

    rref(matrix, rows, cols);

    int equations = 0;
    for (int i = 0; i < rows; i++) {
        bool iseq = false;
        for (int j = 0; j < cols - 1; j++) {
            if (matrix[i][j] != 0) {
                iseq = true;
                break;
            }
        }
        if (iseq) equations++;
        else if (matrix[i][cols-1] != 0) {
            // invalid equation
            no_solutions();
            return 0;
        }
    }

    // more equations than variables
    if (equations > cols - 1) {
        no_solutions();
    }
    // perfectly solved
    else if (equations == cols - 1) {
        // read off results
        one_solution(matrix, cols - 1);
    }
    // undersolved (free variables)
    else {
        multiple_solutions(matrix, cols - 1, rows);
    }

    print_matrix(matrix, rows, cols);
}

// what if the first row has all 0s, AND the first column has all zeros (that variable not used) => then a useless row would stay on top
// is that fine?