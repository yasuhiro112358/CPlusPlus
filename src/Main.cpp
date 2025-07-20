#include <iostream>
#include "calculator.h"

using namespace std;

int main() {
    Calculator calc;
    cout << "2 + 3 = " << calc.add(2, 3) << endl;
    cout << "5 - 3 = " << calc.subtract(5, 3) << endl;
    return 0;
}
