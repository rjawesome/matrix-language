#include "utils.h"

int operating_line = 0;

int gcd(int a, int b) {
    a = abs(a);
    b = abs(b);
    if (!a) return b;
    if (!b) return a;

    // both numbers even
    int k = 0;
    while (!(a & 1) && !(b & 1)) {
        a >>= 1;
        b >>= 1;
        k++;
    }

    // "a" = designated odd number
    while (!(a & 1)) {
        a >>= 1;
    }

    while (b) {
        while (!(b & 1)) b >>= 1;
        if (a > b) {
            int t = a;
            a = b;
            b = t;
        }
        b -= a;
    }

    return a << k;
}

pair<int, int> reduce_sqrt(int s) {
    int whole = 1;
    for (int i = 2; i <= sqrt(s); i++) {
        int sq = (int)pow(i, 2);
        if (s % sq == 0) {
            whole *= i;
            s /= sq;
        }
    }
    return {whole, s};
}

pair<int, int> reduce_sqrts(int s1, int s2) {
    int g = gcd(s1, s2);
    return {g, s1 * s2 / (g*g)};
}