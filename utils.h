#include <bits/stdc++.h>
using namespace std;

const string DASH = "-------------";
const char VAR_NAMES[] = {'x','y','z','w','a','b','c'};
const int VAR_NAME_COUNT = sizeof(VAR_NAMES)/sizeof(VAR_NAMES[0]);

void rref(vector<vector<double>>& matrix, int rows, int cols);
void print_matrix(vector<vector<double>>& matrix, int rows, int cols);
void print_vector(vector<double>& vector, string prefix);
