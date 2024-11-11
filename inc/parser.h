#ifndef PARSER
#define PARSER

#include <set>
#include <map>
#include <queue>
#include <stack>
#include <string>
#include "types.h"
using namespace std;

variant<Error, Expression> parseString(string s);

#endif