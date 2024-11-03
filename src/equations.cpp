#include "equations.h"

void no_solutions() {
    cout << "No solutions!" << endl;
}

void one_solution(vector<vector<Fraction>>& matrix, int variables) {
    cout << "One solution!" << endl;
    vector<Fraction> solution(variables);
    int cur_pos = 0;
    for (int i = 0; i < matrix.size() && cur_pos < variables; i++) {
        if (matrix[i][cur_pos].numerator != 0) {
            solution[cur_pos] = matrix[i][variables];
            cur_pos++;
        }
    }
    print_vector(solution);
}

void multiple_solutions(vector<vector<Fraction>>& matrix, int variables) {
    int rows = matrix.size();
    // free var col # => vector
    map<int, vector<Fraction>> solutions;
    vector<Fraction> offset(variables);
    for (int i = 0; i < variables; i++) {
        if (i >= rows || matrix[i][i].numerator == 0) {
            vector<Fraction> solution(variables);
            solution[i] = {1, 1}; // each free variable contains itself
            solutions[i] = solution;
        }
    }
    for (int i = 0; i < rows; i++) {
        // find the determined variable
        if (matrix[i][i].numerator == 0) continue;
        // update each free var sol if needed
        for (int j = 0; j < variables; j++) {
            if (matrix[i][j].numerator != 0 && solutions.find(j) != solutions.end()) {
                solutions[j][i] = negate_frac(mul_frac(matrix[i][j], inverse_frac(matrix[i][i])));
            }
        }

        // update the offset (end of equation / coefficient)
        offset[i] = mul_frac(matrix[i][variables], inverse_frac(matrix[i][i]));
    }

    cout << "Multiple solutions!" << endl;
    int cnt = 0;
    for (auto [_, solution] : solutions) {
        string variable = VAR_NAMES[cnt%VAR_NAME_COUNT] + to_string((int)(cnt/VAR_NAME_COUNT));
        cout << variable << endl;
        print_vector(solution);
        cnt++;
    }
    cout << "Offset" << endl;
    print_vector(offset);
}

void solve_equations (const vector<vector<Fraction>>& original) {
    vector<vector<Fraction>> matrix = rref(original);
    print_matrix(matrix);

    int rows = matrix.size();
    int cols = matrix[0].size();

    int equations = 0;
    for (int i = 0; i < rows; i++) {
        bool iseq = false;
        for (int j = 0; j < cols - 1; j++) {
            if (matrix[i][j].numerator != 0) {
                iseq = true;
                break;
            }
        }
        if (iseq) equations++;
        else if (matrix[i][cols-1].numerator != 0) {
            // invalid equation
            no_solutions();
            return;
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
        multiple_solutions(matrix, cols - 1);
    }

    return;
}