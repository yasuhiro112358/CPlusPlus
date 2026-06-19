#include "exception_demos.h"
#include <exception>
#include <iostream>

int main() {
  // 最外の安全網：throw を書いていなくても std::bad_alloc 等は起こりうる。
  // main から例外を漏らさない（std::terminate を避ける）ための保険。
  try {
    try_catch::runAllDemos();
    return 0;
  } catch (const std::exception& e) {
    std::cerr << "予期しない例外が main に到達: " << e.what() << '\n';
    return 1;
  } catch (...) {
    std::cerr << "予期しない非標準例外が main に到達" << '\n';
    return 1;
  }
}
