#pragma once
#include <array>
#include <gsl/util>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "game.h"
#include "information_set.h"

namespace cfr {
// ゲーム非依存の CFR エンジン。Game concept を満たす任意のゲームを解く。
// std::sort が比較関数で任意の型を並べ替えるのと同じ発想。
template <Game G>
class CfrSolver {
 public:
  explicit CfrSolver(G game) : game_(std::move(game)) {}

  // iterations 回学習し、プレイヤー0視点の平均ゲーム価値を返す。
  double Train(int iterations) {
    const std::vector<typename G::State> roots = game_.InitialStates();
    double util_sum = 0.0;
    for (int i = 0; i < iterations; ++i) {
      for (const auto& root : roots) {
        util_sum += Cfr(root, 1.0, 1.0);
      }
    }
    return util_sum / (iterations * static_cast<double>(roots.size()));
  }

  // 学習後、各情報集合（キーと平均戦略を持つノード）を訪問する。表示は呼び出し側の責務。
  void ForEachInfoSet(auto&& visit) const {
    for (const auto& [key, node] : node_map_) {
      visit(key, node);
    }
  }

 private:
  static constexpr int kN = G::kNumActions;

  // ゲーム木を辿り、手番プレイヤー視点の利得を返す。p0/p1 は各人の到達確率。
  double Cfr(const G::State& state, double p0, double p1) {
    // 1. 終端なら利得を返す
    if (state.IsTerminal()) {
      return state.Utility();
    }

    // 2. この情報集合のノードを取得（無ければ作られる）
    const int player = state.CurrentPlayer();
    const int opponent = 1 - player;
    InformationSet<kN>& node = node_map_[state.InfoSetKey()];

    // 3. 現在の戦略で各行動の利得と、その期待値を求める
    const std::array<double, kN> strategy =
        node.GetStrategy(ReachOf(player, p0, p1));
    const std::array<double, kN> util =
        ActionUtilities(state, strategy, p0, p1);
    const double node_util = ExpectedUtility(strategy, util);

    // 4. counterfactual regret を積算する（相手の到達確率で重み付け）
    AccumulateRegrets(node, util, node_util, ReachOf(opponent, p0, p1));
    return node_util;
  }

  // 各行動を1手進めた先の利得（手番プレイヤー視点）を再帰で求める。
  std::array<double, kN> ActionUtilities(const G::State& state,
                                         const std::array<double, kN>& strategy,
                                         double p0, double p1) {
    const int player = state.CurrentPlayer();
    std::array<double, kN> util{};
    for (int a = 0; a < kN; ++a) {
      const typename G::State next = state.Next(a);
      // 子は相手の手番。相手視点の利得を反転して自分視点に揃える。
      // 自分が選んだ枝の到達確率だけ strategy[a] 倍する。
      gsl::at(util, a) = (player == 0)
                             ? -Cfr(next, p0 * gsl::at(strategy, a), p1)
                             : -Cfr(next, p0, p1 * gsl::at(strategy, a));
    }
    return util;
  }

  // 指定プレイヤーの到達確率を選ぶ。
  static double ReachOf(int player, double p0, double p1) {
    return player == 0 ? p0 : p1;
  }

  // 戦略で重み付けした期待利得（strategy と util の内積）。
  static double ExpectedUtility(const std::array<double, kN>& strategy,
                                const std::array<double, kN>& util) {
    double sum = 0.0;
    for (int a = 0; a < kN; ++a) {
      sum += gsl::at(strategy, a) * gsl::at(util, a);
    }
    return sum;
  }

  // counterfactual regret を積算する（相手の到達確率で重み付け）。
  static void AccumulateRegrets(InformationSet<kN>& node,
                                const std::array<double, kN>& util,
                                double node_util, double counterfactual_reach) {
    for (int a = 0; a < kN; ++a) {
      const double regret = gsl::at(util, a) - node_util;
      node.AccumulateRegret(a, counterfactual_reach * regret);
    }
  }

  G game_;
  // 情報集合のキー → ノード。map で順序付き（出力が読みやすい）。
  std::map<std::string, InformationSet<kN>> node_map_;
};
}  // namespace cfr
