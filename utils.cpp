#include "utils.h"

int gcd(int a, int b) {
    a = abs(a);
    b = abs(b);
    if (a < b) {
        int temp = a;
        a = b;
        b = temp;
    }
    while (b != 0) {
        int nb = a % b;
        a = b;
        b = nb;
    }
    return a;
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