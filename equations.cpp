#include <bits/stdc++.h>
#include "utils.h"

using namespace std;

void no_solutions() {
    cout << "No solutions!" << endl;
}

void one_solution(vector<vector<double>>& matrix, int variables) {
    cout << "One solution!" << endl;
    vector<double> solution(variables);
    for (int i = 0; i < variables; i++) solution[i] = matrix[i][variables];
    print_vector(solution, "");
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
        string variable = VAR_NAMES[cnt%VAR_NAME_COUNT] + to_string((int)(cnt/VAR_NAME_COUNT));
        print_vector(solution, variable);
        cnt++;
    }
    print_vector(offset, "Offset");
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

    // print_matrix(matrix, rows, cols);
}