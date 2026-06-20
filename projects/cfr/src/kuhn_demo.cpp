#include "kuhn_demo.h"

#include <array>
#include <gsl/util>
#include <iomanip>
#include <iostream>
#include <string>

#include "cfr_solver.h"
#include "games/kuhn_game.h"
#include "games/kuhn_state.h"

namespace cfr {
void RunKuhnDemo() {
  CfrSolver<KuhnGame> solver{KuhnGame{}};
  const int iterations = 100000;
  const double game_value = solver.Train(iterations);
  const double theoretical_value = -1.0 / 18.0;  // 既知の理論値 -1/18

  std::cout << "反復回数            : " << iterations << '\n';
  std::cout << "P0 の平均ゲーム価値 : " << game_value << '\n';
  std::cout << "理論値 -1/18        : " << theoretical_value << '\n';

  std::cout << "--- 平均戦略（情報集合: pass/bet）---\n";
  std::cout << std::fixed << std::setprecision(4);
  solver.ForEachInfoSet([](const std::string& key, const auto& node) {
    const std::array<double, KuhnGame::kNumActions> avg =
        node.GetAverageStrategy();
    std::cout << std::left << std::setw(3) << key
              << " : pass=" << gsl::at(avg, 0) << " bet=" << gsl::at(avg, 1)
              << '\n';
  });
}
}  // namespace cfr
