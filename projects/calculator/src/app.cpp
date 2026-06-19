#include "app.h"
#include "arithmetic.h"
#include <iostream>

namespace calculator {
  App::App() = default;

  void App::run() {
    std::cout << "Hello from " << appName_ << "!" << '\n';

    // Calculator は自由関数になったので、オブジェクトを作らず直接呼ぶ
    std::cout << "2 + 3 = " << add(2, 3) << '\n';
    std::cout << "5 - 3 = " << subtract(5, 3) << '\n';
  }
} // namespace calculator
