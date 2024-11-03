#ifndef PARSER
#define PARSER

#include "fraction.h"
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include <memory>

using namespace std;

void process_expression();

enum DataType {
    TYPE_FRACTION,
    TYPE_VECTOR,
    TYPE_MATRIX,
    TYPE_FUNCTION
};

struct DataContainer {
    DataType type;
    bool anon = true;
    union {
        Fraction data;
        void* ptr;
    };
};

#endif