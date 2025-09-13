#include "cpp_lab/app.h"
#include <iostream>
#include <utility>

namespace cpp_lab {
  // Initialize static member
  std::string App::appName_ = "cpp-lab";

  // Constructor
  App::App() = default;

  // Run the application
  void App::run() {
    std::cout << "Hello from " << appName_ << "!!!" << std::endl;
  }

} // namespace cpp_lab
