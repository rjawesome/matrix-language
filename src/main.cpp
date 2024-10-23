#include "main.h"

int main () {
    string o; cin >> o;
    if (o.compare("gram") == 0) {
        int rows, cols; cin >> rows >> cols;
        vector<vector<Fraction>> matrix(rows, vector<Fraction>(cols));
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cin >> matrix[i][j];
            }
        }

        matrix = orthonormalize_rows(matrix);

        print_matrix(matrix);
        return 0;
    } else if (o.compare("eq") == 0) {
        return solve_equations();
    }
    cout << "Invalid operation" << endl;
    return 1;
}