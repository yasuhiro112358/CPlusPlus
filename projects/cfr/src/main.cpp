#include <exception>
#include <iostream>

#include "kuhn_demo.h"

int main() {
  try {
    cfr::RunKuhnDemo();
    return 0;
  } catch (const std::exception& e) {
    std::cerr << "予期しない例外が main に到達: " << e.what() << '\n';
    return 1;
  } catch (...) {
    std::cerr << "予期しない非標準例外が main に到達" << '\n';
    return 1;
  }
}
