#ifndef UTILS
#define UTILS

#define expect(x, y) do { if (!(x)) return Error{y,operating_line}; } while (false)
#define unwrap(t, x, y) do { auto temp = y; if (holds_alternative<Error>(temp)) {return get<Error>(temp);} else { x = get<t>(temp); } } while (false)

using namespace std;
#include "fraction.h"
#include "math.h"
#include <variant>
#include <immintrin.h>
#include <chrono>
#include <vector>

const string DASH = "-------------";
const char VAR_NAMES[] = {'x','y','z','w','a','b','c'};
const int VAR_NAME_COUNT = sizeof(VAR_NAMES)/sizeof(VAR_NAMES[0]);

int gcd(int a, int b);
__m256i gcd(__m256i a, __m256i b);
__m256i divide_m256i_exact(__m256i dividend_vec, __m256i divisor_vec);
pair<int, int> reduce_sqrt(int s);
pair<int, int> reduce_sqrts(int s1, int s2);
pair<__m256i, __m256i> reduce_sqrts(__m256i s1, __m256i s2);
void print_m256(__m256i vec);

extern int operating_line;

struct Error {
    string_view message;
    int line;
};

#endif