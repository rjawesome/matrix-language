#include "interpreted_funcs.h"

variant<string_view, DataContainer> get_matrix_int(DataContainer args[]) {
    vector<vector<Fraction>>* matrix = new vector<vector<Fraction>>(move(get_matrix()));
    return DataContainer{TYPE_MATRIX, true, .ptr = matrix};
}

variant<string_view, DataContainer> rref_int(DataContainer args[]) {
    vector<vector<Fraction>> input = *(vector<vector<Fraction>>*)(args[0].ptr);
    vector<vector<Fraction>>* matrix = new vector<vector<Fraction>>(move(rref(input)));
    return DataContainer{TYPE_MATRIX, true, .ptr = matrix};
}

variant<string_view, DataContainer> solve_equations_int(DataContainer args[]) {
    vector<vector<Fraction>> input = *(vector<vector<Fraction>>*)(args[0].ptr);
    solve_equations(input);
    return DataContainer{TYPE_NONE, true};
}

variant<string_view, DataContainer> orthonormalize_int(DataContainer args[]) {
    vector<vector<Fraction>> input = *(vector<vector<Fraction>>*)(args[0].ptr);
    vector<vector<Fraction>> output = transpose(orthonormalize_rows(transpose(input)));
    vector<vector<Fraction>>* matrix = new vector<vector<Fraction>>(move(output));
    return DataContainer{TYPE_MATRIX, true, .ptr = matrix};
}

variant<string_view, DataContainer> qr_int(DataContainer args[]) {
    vector<vector<Fraction>> input = *(vector<vector<Fraction>>*)(args[0].ptr);
    vector<vector<Fraction>> qT = orthonormalize_rows(transpose(input));
    vector<vector<Fraction>> Q = transpose(qT);
    vector<vector<Fraction>> R = mat_mul(qT, input);
    vector<vector<Fraction>>* matrix = new vector<vector<Fraction>>(move(R));
    return DataContainer{TYPE_MATRIX, true, .ptr = matrix};
}

map<string, DataContainer> base_global_frame = {
    {"get_matrix", {TYPE_FUNCTION, false, .function = {0, {}, TYPE_MATRIX, get_matrix_int} }},
    {"rref", {TYPE_FUNCTION, false, .function = {1, {TYPE_MATRIX}, TYPE_MATRIX, rref_int }}},
    {"solve_equations", {TYPE_FUNCTION, false, .function = {1, {TYPE_MATRIX}, TYPE_NONE, solve_equations_int }}},
    {"ortho", {TYPE_FUNCTION, false, .function = {1, {TYPE_MATRIX}, TYPE_MATRIX, orthonormalize_int }}},
    {"qr", {TYPE_FUNCTION, false, .function = {1, {TYPE_MATRIX}, TYPE_MATRIX, qr_int }}}
};
