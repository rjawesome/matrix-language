#ifndef UTILS
#define UTILS

#define expect(x, y) do { if (!(x)) return y; } while (false);

#include "fraction.h"
#include "math.h"
#include <variant>
using namespace std;

const string DASH = "-------------";
const char VAR_NAMES[] = {'x','y','z','w','a','b','c'};
const int VAR_NAME_COUNT = sizeof(VAR_NAMES)/sizeof(VAR_NAMES[0]);

int gcd(int a, int b);
pair<int, int> reduce_sqrt(int s);

#endif