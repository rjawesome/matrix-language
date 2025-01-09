#include "interpreted_funcs.h"

variant<Error, DataContainer> get_matrix(DataContainer args[]) {
    expect(args[0].frac.denominator == 1 && args[1].frac.denominator == 1 && args[0].frac.sqrt == 1 && args[1].frac.sqrt == 1 && args[0].frac.numerator > 0 && args[1].frac.numerator > 0, "Matrix dimensions must be a positive integer");
    variant<Error, vector<vector<Fraction>>> matrix = get_matrix(args[0].frac.numerator, args[1].frac.numerator);
    return holds_alternative<Error>(matrix) ? 
        variant<Error, DataContainer>(get<Error>(matrix)) :
        DataContainer{TYPE_MATRIX, true, .ptr = new vector<vector<Fraction>>(move(get<vector<vector<Fraction>>>(matrix)))};
}

variant<Error, DataContainer> rref(DataContainer args[]) {
    vector<vector<Fraction>> input = *(vector<vector<Fraction>>*)(args[0].ptr);
    variant<Error, vector<vector<Fraction>>> matrix = rref(input);
    return holds_alternative<Error>(matrix) ? 
        variant<Error, DataContainer>(get<Error>(matrix)) :
        DataContainer{TYPE_MATRIX, true, .ptr = new vector<vector<Fraction>>(move(get<vector<vector<Fraction>>>(matrix)))};
}

variant<Error, DataContainer> solve_equations(DataContainer args[]) {
    vector<vector<Fraction>> input = *(vector<vector<Fraction>>*)(args[0].ptr);
    Error err = solve_equations(input);
    if (err.message.size() > 0) return err;
    return DataContainer{TYPE_NONE, true};
}

variant<Error, DataContainer> orthonormalize(DataContainer args[]) {
    vector<vector<Fraction>> input = *(vector<vector<Fraction>>*)(args[0].ptr);
    variant<Error, vector<vector<Fraction>>> matrix = orthonormalize_rows(transpose(input));
    if (holds_alternative<vector<vector<Fraction>>>(matrix)) matrix = transpose(get<vector<vector<Fraction>>>(matrix));
    return holds_alternative<Error>(matrix) ? 
        variant<Error, DataContainer>(get<Error>(matrix)) :
        DataContainer{TYPE_MATRIX, true, .ptr = new vector<vector<Fraction>>(move(get<vector<vector<Fraction>>>(matrix)))};
}

variant<Error, DataContainer> inverse(DataContainer args[]) {
    if (args[0].type == TYPE_MATRIX) {
        vector<vector<Fraction>> input = *(vector<vector<Fraction>>*)(args[0].ptr);
        variant<Error, vector<vector<Fraction>>> matrix = inverse(input);
        return holds_alternative<Error>(matrix) ? 
            variant<Error, DataContainer>(get<Error>(matrix)) :
            DataContainer{TYPE_MATRIX, true, .ptr = new vector<vector<Fraction>>(move(get<vector<vector<Fraction>>>(matrix)))};
    }
    if (args[0].type == TYPE_FRACTION) {
        expect(args[0].frac.numerator != 0, "Cannot invert zero");
        return DataContainer{TYPE_FRACTION, true, .frac = inverse_frac(args[0].frac)};
    }
    expect(false, "Inverse not supported for this type");
}

variant<Error, DataContainer> determinant(DataContainer args[]) {
    vector<vector<Fraction>> input = *(vector<vector<Fraction>>*)(args[0].ptr);
    variant<Error, Fraction> determ = determinant(input);
    return holds_alternative<Error>(determ) ? 
        variant<Error, DataContainer>(get<Error>(determ)) :
        DataContainer{TYPE_FRACTION, true, .frac = get<Fraction>(determ)};
}

variant<Error, DataContainer> qr(DataContainer args[]) {
    vector<vector<Fraction>> input = *(vector<vector<Fraction>>*)(args[0].ptr);
    variant<Error, vector<vector<Fraction>>> qT_res = orthonormalize_rows(transpose(input));
    if (holds_alternative<Error>(qT_res)) return get<Error>(qT_res);
    auto qT = get<vector<vector<Fraction>>>(qT_res);
    vector<vector<Fraction>> Q = transpose(qT);
    vector<vector<Fraction>> R = get<vector<vector<Fraction>>>(mat_mul(qT, input));
    vector<vector<Fraction>>* matrix = new vector<vector<Fraction>>(move(R));
    return DataContainer{TYPE_MATRIX, true, .ptr = matrix};
}

variant<Error, DataContainer> add(DataContainer args[]) {
    if (args[0].type == TYPE_FRACTION && args[1].type == TYPE_FRACTION) {
        variant<Error, Fraction> sum = add_frac(args[0].frac, args[1].frac);
        return holds_alternative<Error>(sum) ? 
            variant<Error, DataContainer>(get<Error>(sum)) : 
            DataContainer{TYPE_FRACTION, true, .frac = get<Fraction>(sum)};
    }
    if (args[0].type == TYPE_MATRIX && args[1].type == TYPE_MATRIX) {
        vector<vector<Fraction>> matrix1 = *(vector<vector<Fraction>>*)(args[0].ptr);
        vector<vector<Fraction>> matrix2 = *(vector<vector<Fraction>>*)(args[1].ptr);
        variant<Error, vector<vector<Fraction>>> matrix = add_matrix(matrix1, matrix2);
        return holds_alternative<Error>(matrix) ? 
            variant<Error, DataContainer>(get<Error>(matrix)) :
            DataContainer{TYPE_MATRIX, true, .ptr = new vector<vector<Fraction>>(move(get<vector<vector<Fraction>>>(matrix)))};
    }
    expect(false, "Addition between types not supported");
}

variant<Error, DataContainer> mult(DataContainer args[]) {
    if (args[0].type == TYPE_FRACTION && args[1].type == TYPE_FRACTION) {
        return DataContainer{TYPE_FRACTION, true, .frac = mul_frac(args[0].frac, args[1].frac)};
    }
    if (args[0].type == TYPE_MATRIX && args[1].type == TYPE_MATRIX) {
        vector<vector<Fraction>> matrix1 = *(vector<vector<Fraction>>*)(args[0].ptr);
        vector<vector<Fraction>> matrix2 = *(vector<vector<Fraction>>*)(args[1].ptr);
        variant<Error, vector<vector<Fraction>>> matrix = mat_mul_simd(matrix1, matrix2);
        return holds_alternative<Error>(matrix) ? 
            variant<Error, DataContainer>(get<Error>(matrix)) :
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
    expect(false, "Multiplication between types not supported");
}

variant<Error, DataContainer> transpose(DataContainer args[]) {
    vector<vector<Fraction>> matrix1 = *(vector<vector<Fraction>>*)(args[0].ptr);
    return DataContainer{TYPE_MATRIX, true, .ptr = new vector<vector<Fraction>>(move(transpose(matrix1)))};
}

variant<Error, DataContainer> rows(DataContainer args[]) {
    vector<vector<Fraction>> matrix1 = *(vector<vector<Fraction>>*)(args[0].ptr);
    return DataContainer{TYPE_FRACTION, true, .frac = {(int)matrix1.size(), 1, 1}};
}

variant<Error, DataContainer> cols(DataContainer args[]) {
    vector<vector<Fraction>> matrix1 = *(vector<vector<Fraction>>*)(args[0].ptr);
    return DataContainer{TYPE_FRACTION, true, .frac = {(int)(matrix1.size() > 0 ? matrix1[0].size(): 0), 1, 1}};
}

variant<Error, DataContainer> norm_sq(DataContainer args[]) {
    vector<vector<Fraction>> matrix1 = *(vector<vector<Fraction>>*)(args[0].ptr);
    return DataContainer{TYPE_FRACTION, true, .frac = norm_sq(matrix1)};
}

map<string, DataContainer> base_global_frame = {
    {"get_matrix", {TYPE_FUNCTION, false, .function = {2, {TYPE_FRACTION, TYPE_FRACTION}, TYPE_MATRIX, get_matrix} }},
    {"rref", {TYPE_FUNCTION, false, .function = {1, {TYPE_MATRIX}, TYPE_MATRIX, rref }}},
    {"solve_equations", {TYPE_FUNCTION, false, .function = {1, {TYPE_MATRIX}, TYPE_NONE, solve_equations }}},
    {"ortho", {TYPE_FUNCTION, false, .function = {1, {TYPE_MATRIX}, TYPE_MATRIX, orthonormalize }}},
    {"qr", {TYPE_FUNCTION, false, .function = {1, {TYPE_MATRIX}, TYPE_MATRIX, qr }}},
    {"rows", {TYPE_FUNCTION, false, .function = {1, {TYPE_MATRIX}, TYPE_FRACTION, rows }}},
    {"cols", {TYPE_FUNCTION, false, .function = {1, {TYPE_MATRIX}, TYPE_FRACTION, cols }}},
    {"transpose", {TYPE_FUNCTION, false, .function = {1, {TYPE_MATRIX}, TYPE_MATRIX, transpose }}},
    {"inverse", {TYPE_FUNCTION, false, .function = {1, {TYPE_ANY}, TYPE_ANY, inverse }}},
    {"determinant", {TYPE_FUNCTION, false, .function = {1, {TYPE_MATRIX}, TYPE_FRACTION, determinant }}},
    {"norm_sq", {TYPE_FUNCTION, false, .function = {1, {TYPE_MATRIX}, TYPE_FRACTION, norm_sq }}},
    {"+", {TYPE_FUNCTION, false, .function = {2, {TYPE_ANY, TYPE_ANY}, TYPE_ANY, add }}},
    {"*", {TYPE_FUNCTION, false, .function = {2, {TYPE_ANY, TYPE_ANY}, TYPE_ANY, mult }}}
};
