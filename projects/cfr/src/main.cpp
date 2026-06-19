#include "kuhn_trainer.h"
#include <exception>
#include <iostream>

int main() {
  try {
    cfr::KuhnTrainer trainer;
    const int iterations = 100000;
    const double gameValue = trainer.train(iterations);

    std::cout << "反復回数            : " << iterations << '\n';
    std::cout << "P0 の平均ゲーム価値 : " << gameValue << '\n';
    std::cout << "理論値 -1/18        : " << -1.0 / 18.0 << '\n';
    trainer.printStrategy();
    return 0;
  } catch (const std::exception& e) {
    std::cerr << "予期しない例外が main に到達: " << e.what() << '\n';
    return 1;
  } catch (...) {
    std::cerr << "予期しない非標準例外が main に到達" << '\n';
    return 1;
  }
}
