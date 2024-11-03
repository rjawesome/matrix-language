#ifndef PARSER
#define PARSER

#include "fraction.h"
#include "matrix.h"
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

struct DataContainer;

struct Function {
    int arglen;
    DataType args[10];
    DataType returnType;
    DataContainer (*func)(DataContainer[]);
};

struct DataContainer {
    DataType type;
    bool anon = true;
    union {
        Fraction frac;
        Function function;
        void* ptr;
    };
};

#endif