#include "calculator.h"
#include "cpp_lab/app.h"
#include <iostream>

int main() {
  std::cout << "Hello world!" << std::endl;

  Calculator calc;
  std::cout << "2 + 3 = " << calc.add(2, 3) << std::endl;
  std::cout << "5 - 3 = " << calc.subtract(5, 3) << std::endl;

  cpp_lab::App app;
  app.run();

  return 0;
}
