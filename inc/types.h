#ifndef TYPES
#define TYPES

#include <vector>
#include <string>
#include "fraction.h"

struct Expression {
    string name;
    int line;
    vector<Expression> children;
};

enum DataType {
    TYPE_FRACTION,
    TYPE_VECTOR,
    TYPE_MATRIX,
    TYPE_FUNCTION,
    TYPE_NONE,
    TYPE_ANY // should not be applied to any actual data container
};

struct DataContainer;

struct Function {
    int arglen;
    DataType args[10];
    DataType returnType;
    variant<Error, DataContainer> (*func)(DataContainer[]);
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