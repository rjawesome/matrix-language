#include "interpreted_funcs.h"

variant<string_view, DataContainer> get_matrix(DataContainer args[]) {
    variant<string_view, vector<vector<Fraction>>> matrix = get_matrix();
    return holds_alternative<string_view>(matrix) ? 
        variant<string_view, DataContainer>(get<string_view>(matrix)) :
        DataContainer{TYPE_MATRIX, true, .ptr = new vector<vector<Fraction>>(move(get<vector<vector<Fraction>>>(matrix)))};
}

variant<string_view, DataContainer> rref(DataContainer args[]) {
    vector<vector<Fraction>> input = *(vector<vector<Fraction>>*)(args[0].ptr);
    variant<string_view, vector<vector<Fraction>>> matrix = rref(input);
    return holds_alternative<string_view>(matrix) ? 
        variant<string_view, DataContainer>(get<string_view>(matrix)) :
        DataContainer{TYPE_MATRIX, true, .ptr = new vector<vector<Fraction>>(move(get<vector<vector<Fraction>>>(matrix)))};
}

variant<string_view, DataContainer> solve_equations(DataContainer args[]) {
    vector<vector<Fraction>> input = *(vector<vector<Fraction>>*)(args[0].ptr);
    solve_equations(input);
    return DataContainer{TYPE_NONE, true};
}

variant<string_view, DataContainer> orthonormalize(DataContainer args[]) {
    vector<vector<Fraction>> input = *(vector<vector<Fraction>>*)(args[0].ptr);
    variant<string_view, vector<vector<Fraction>>> matrix = orthonormalize_rows(transpose(input));
    if (holds_alternative<vector<vector<Fraction>>>(matrix)) matrix = transpose(get<vector<vector<Fraction>>>(matrix));
    return holds_alternative<string_view>(matrix) ? 
        variant<string_view, DataContainer>(get<string_view>(matrix)) :
        DataContainer{TYPE_MATRIX, true, .ptr = new vector<vector<Fraction>>(move(get<vector<vector<Fraction>>>(matrix)))};
}

variant<string_view, DataContainer> inverse(DataContainer args[]) {
    vector<vector<Fraction>> input = *(vector<vector<Fraction>>*)(args[0].ptr);
    variant<string_view, vector<vector<Fraction>>> matrix = inverse(input);
    return holds_alternative<string_view>(matrix) ? 
        variant<string_view, DataContainer>(get<string_view>(matrix)) :
        DataContainer{TYPE_MATRIX, true, .ptr = new vector<vector<Fraction>>(move(get<vector<vector<Fraction>>>(matrix)))};
}

variant<string_view, DataContainer> determinant(DataContainer args[]) {
    vector<vector<Fraction>> input = *(vector<vector<Fraction>>*)(args[0].ptr);
    variant<string_view, Fraction> determ = determinant(input);
    return holds_alternative<string_view>(determ) ? 
        variant<string_view, DataContainer>(get<string_view>(determ)) :
        DataContainer{TYPE_FRACTION, true, .frac = get<Fraction>(determ)};
}

variant<string_view, DataContainer> qr(DataContainer args[]) {
    vector<vector<Fraction>> input = *(vector<vector<Fraction>>*)(args[0].ptr);
    variant<string_view, vector<vector<Fraction>>> qT_res = orthonormalize_rows(transpose(input));
    if (holds_alternative<string_view>(qT_res)) return get<string_view>(qT_res);
    auto qT = get<vector<vector<Fraction>>>(qT_res);
    vector<vector<Fraction>> Q = transpose(qT);
    vector<vector<Fraction>> R = get<vector<vector<Fraction>>>(mat_mul(qT, input));
    vector<vector<Fraction>>* matrix = new vector<vector<Fraction>>(move(R));
    return DataContainer{TYPE_MATRIX, true, .ptr = matrix};
}

variant<string_view, DataContainer> add(DataContainer args[]) {
    if (args[0].type == TYPE_FRACTION && args[1].type == TYPE_FRACTION) {
        return DataContainer{TYPE_FRACTION, true, .frac = add_frac(args[0].frac, args[1].frac)};
    }
    if (args[0].type == TYPE_MATRIX && args[1].type == TYPE_MATRIX) {
        vector<vector<Fraction>> matrix1 = *(vector<vector<Fraction>>*)(args[0].ptr);
        vector<vector<Fraction>> matrix2 = *(vector<vector<Fraction>>*)(args[1].ptr);
        variant<string_view, vector<vector<Fraction>>> matrix = add_matrix(matrix1, matrix2);
        return holds_alternative<string_view>(matrix) ? 
            variant<string_view, DataContainer>(get<string_view>(matrix)) :
            DataContainer{TYPE_MATRIX, true, .ptr = new vector<vector<Fraction>>(move(get<vector<vector<Fraction>>>(matrix)))};
    }
    return "Addition between types not supported";
}

variant<string_view, DataContainer> mult(DataContainer args[]) {
    if (args[0].type == TYPE_FRACTION && args[1].type == TYPE_FRACTION) {
        return DataContainer{TYPE_FRACTION, true, .frac = mul_frac(args[0].frac, args[1].frac)};
    }
    if (args[0].type == TYPE_MATRIX && args[1].type == TYPE_MATRIX) {
        vector<vector<Fraction>> matrix1 = *(vector<vector<Fraction>>*)(args[0].ptr);
        vector<vector<Fraction>> matrix2 = *(vector<vector<Fraction>>*)(args[1].ptr);
        variant<string_view, vector<vector<Fraction>>> matrix = mat_mul(matrix1, matrix2);
        return holds_alternative<string_view>(matrix) ? 
            variant<string_view, DataContainer>(get<string_view>(matrix)) :
            DataContainer{TYPE_MATRIX, true, .ptr = new vector<vector<Fraction>>(move(get<vector<vector<Fraction>>>(matrix)))};
    }
    if (args[0].type == TYPE_MATRIX && args[1].type == TYPE_FRACTION) {
        DataContainer temp = args[0];
        args[0] = args[1];
        args[1] = temp;
    }
    if (args[0].type == TYPE_FRACTION && args[1].type == TYPE_MATRIX) {
        vector<vector<Fraction>> matrix1 = *(vector<vector<Fraction>>*)(args[1].ptr);
        return DataContainer{TYPE_MATRIX, true, .ptr = new vector<vector<Fraction>>(move(matrix_scale(args[0].frac, matrix1))) };
    }
    return "Multiplication between types not supported";
}

variant<string_view, DataContainer> transpose(DataContainer args[]) {
    vector<vector<Fraction>> matrix1 = *(vector<vector<Fraction>>*)(args[0].ptr);
    return DataContainer{TYPE_MATRIX, true, .ptr = new vector<vector<Fraction>>(move(transpose(matrix1)))};
}

variant<string_view, DataContainer> rows(DataContainer args[]) {
    vector<vector<Fraction>> matrix1 = *(vector<vector<Fraction>>*)(args[0].ptr);
    return DataContainer{TYPE_FRACTION, true, .frac = {(int)matrix1.size(), 1, 1}};
}

variant<string_view, DataContainer> cols(DataContainer args[]) {
    vector<vector<Fraction>> matrix1 = *(vector<vector<Fraction>>*)(args[0].ptr);
    return DataContainer{TYPE_FRACTION, true, .frac = {(int)(matrix1.size() > 0 ? matrix1[0].size(): 0), 1, 1}};
}

map<string, DataContainer> base_global_frame = {
    {"get_matrix", {TYPE_FUNCTION, false, .function = {0, {}, TYPE_MATRIX, get_matrix} }},
    {"rref", {TYPE_FUNCTION, false, .function = {1, {TYPE_MATRIX}, TYPE_MATRIX, rref }}},
    {"solve_equations", {TYPE_FUNCTION, false, .function = {1, {TYPE_MATRIX}, TYPE_NONE, solve_equations }}},
    {"ortho", {TYPE_FUNCTION, false, .function = {1, {TYPE_MATRIX}, TYPE_MATRIX, orthonormalize }}},
    {"qr", {TYPE_FUNCTION, false, .function = {1, {TYPE_MATRIX}, TYPE_MATRIX, qr }}},
    {"rows", {TYPE_FUNCTION, false, .function = {1, {TYPE_MATRIX}, TYPE_FRACTION, rows }}},
    {"cols", {TYPE_FUNCTION, false, .function = {1, {TYPE_MATRIX}, TYPE_FRACTION, cols }}},
    {"transpose", {TYPE_FUNCTION, false, .function = {1, {TYPE_MATRIX}, TYPE_MATRIX, transpose }}},
    {"inverse", {TYPE_FUNCTION, false, .function = {1, {TYPE_MATRIX}, TYPE_MATRIX, inverse }}},
    {"determinant", {TYPE_FUNCTION, false, .function = {1, {TYPE_MATRIX}, TYPE_FRACTION, determinant }}},
    {"+", {TYPE_FUNCTION, false, .function = {2, {TYPE_ANY, TYPE_ANY}, TYPE_ANY, add }}},
    {"*", {TYPE_FUNCTION, false, .function = {2, {TYPE_ANY, TYPE_ANY}, TYPE_ANY, mult }}}
};
