#include "cpp_lab/app.h"
#include "cpp_lab/calculator.h"
#include <iostream>

using namespace cpp_lab;

// Initialize static member
std::string App::appName_ = "cpp-lab";

// Constructor
App::App() = default;

// Run the application
void App::run() {
  std::cout << "Hello from " << appName_ << "!!!" << std::endl;

  Calculator calc;
  std::cout << "2 + 3 = " << calc.add(2, 3) << std::endl;
  std::cout << "5 - 3 = " << calc.subtract(5, 3) << std::endl;
}
