#include "app.h"

#include <iostream>

#include "arithmetic.h"

namespace calculator {
App::App() = default;

void App::Run() {
  std::cout << "Hello from " << app_name_ << "!" << '\n';

  // Calculator は自由関数になったので、オブジェクトを作らず直接呼ぶ
  const int x = 12;
  const int y = 5;
  std::cout << x << " + " << y << " = " << Add(x, y) << '\n';
  std::cout << x << " - " << y << " = " << Subtract(x, y) << '\n';
}
}  // namespace calculator
