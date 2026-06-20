#include "kuhn_demo.h"
#include "cfr_solver.h"
#include "games/kuhn_game.h"
#include <array>
#include <iomanip>
#include <iostream>
#include <string>

namespace cfr {
  void runKuhnDemo() {
    CfrSolver<KuhnGame> solver{KuhnGame{}};
    const int iterations = 100000;
    const double gameValue = solver.train(iterations);

    std::cout << "反復回数            : " << iterations << '\n';
    std::cout << "P0 の平均ゲーム価値 : " << gameValue << '\n';
    std::cout << "理論値 -1/18        : " << -1.0 / 18.0 << '\n';

    std::cout << "--- 平均戦略（情報集合: pass/bet）---\n";
    std::cout << std::fixed << std::setprecision(4);
    solver.forEachInfoSet([](const std::string& key, const auto& node) {
      const std::array<double, KuhnGame::NUM_ACTIONS> avg =
          node.getAverageStrategy();
      std::cout << std::left << std::setw(3) << key << " : pass=" << avg[0]
                << " bet=" << avg[1] << '\n';
    });
  }
} // namespace cfr
