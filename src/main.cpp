#include "main.h"

int main () {
    string o; cin >> o;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear cin
    process_expressions();
    return 0;
}