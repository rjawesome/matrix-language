#ifndef UTILS
#define UTILS

#include "fraction.h"
#include "math.h"
using namespace std;

const string DASH = "-------------";
const char VAR_NAMES[] = {'x','y','z','w','a','b','c'};
const int VAR_NAME_COUNT = sizeof(VAR_NAMES)/sizeof(VAR_NAMES[0]);

int gcd(int a, int b);
pair<int, int> reduce_sqrt(int s);

#endif