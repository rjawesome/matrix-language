#include "main.h"

int main () {
    string o; cin >> o;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear cin
    if (o.compare("gram") == 0) {
        vector<vector<Fraction>> matrix = get_matrix();
        matrix = transpose(orthonormalize_rows(transpose(matrix)));
        print_matrix(matrix);
        return 0;
    } else if (o.compare("eq") == 0) {
        vector<vector<Fraction>> matrix = get_matrix();
        solve_equations(matrix);
        return 0;
    } else if (o.compare("qr") == 0) {
        vector<vector<Fraction>> matrix = get_matrix();
        vector<vector<Fraction>> qT = orthonormalize_rows(transpose(matrix));
        vector<vector<Fraction>> Q = transpose(qT);
        vector<vector<Fraction>> R = mat_mul(qT, matrix);
        cout << "Q" << endl;
        print_matrix(Q);
        cout << "R" << endl;
        print_matrix(R);
        return 0;
    } else if (o.compare("p") == 0) {
        process_expression();
        return 0;
    }
    cout << "Invalid operation" << endl;
    return 1;
}