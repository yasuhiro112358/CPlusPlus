#include "app.h"
#include "calculator.h"
#include <iostream>

using namespace calculator;

std::string App::appName_ = "calculator";

App::App() = default;

void App::run() {
  std::cout << "Hello from " << appName_ << "!" << std::endl;

  Calculator calc;
  std::cout << "2 + 3 = " << calc.add(2, 3) << std::endl;
  std::cout << "5 - 3 = " << calc.subtract(5, 3) << std::endl;
}
