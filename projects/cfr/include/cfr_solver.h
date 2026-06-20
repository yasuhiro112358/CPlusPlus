#pragma once
#include <array>
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
  double train(int iterations) {
    const std::vector<typename G::State> roots = game_.initialStates();
    double utilSum = 0.0;
    for (int i = 0; i < iterations; ++i) {
      for (const auto& root : roots) {
        utilSum += cfr(root, 1.0, 1.0);
      }
    }
    return utilSum / (iterations * static_cast<double>(roots.size()));
  }

  // 学習後、各情報集合（キーと平均戦略を持つノード）を訪問する。表示は呼び出し側の責務。
  void forEachInfoSet(auto&& visit) const {
    for (const auto& [key, node] : nodeMap_) {
      visit(key, node);
    }
  }

 private:
  static constexpr int N = G::NUM_ACTIONS;

  // ゲーム木を辿り、手番プレイヤー視点の利得を返す。p0/p1 は各人の到達確率。
  double cfr(const G::State& state, double p0, double p1) {
    // 1. 終端なら利得を返す
    if (game_.isTerminal(state)) {
      return game_.utility(state);
    }

    // 2. この情報集合のノードを取得（無ければ作られる）
    const int player = game_.currentPlayer(state);
    const int opponent = 1 - player;
    InformationSet<N>& node = nodeMap_[game_.infoSetKey(state)];

    // 3. 現在の戦略で各行動の利得と、その期待値を求める
    const std::array<double, N> strategy =
        node.getStrategy(reachOf(player, p0, p1));
    const std::array<double, N> util = actionUtilities(state, strategy, p0, p1);
    const double nodeUtil = expectedUtility(strategy, util);

    // 4. counterfactual regret を積算する（相手の到達確率で重み付け）
    accumulateRegrets(node, util, nodeUtil, reachOf(opponent, p0, p1));
    return nodeUtil;
  }

  // 各行動を1手進めた先の利得（手番プレイヤー視点）を再帰で求める。
  std::array<double, N> actionUtilities(const G::State& state,
                                        const std::array<double, N>& strategy,
                                        double p0, double p1) {
    const int player = game_.currentPlayer(state);
    std::array<double, N> util{};
    for (int a = 0; a < N; ++a) {
      const typename G::State next = game_.nextState(state, a);
      // 子は相手の手番。相手視点の利得を反転して自分視点に揃える。
      // 自分が選んだ枝の到達確率だけ strategy[a] 倍する。
      util[a] = (player == 0) ? -cfr(next, p0 * strategy[a], p1)
                              : -cfr(next, p0, p1 * strategy[a]);
    }
    return util;
  }

  // 指定プレイヤーの到達確率を選ぶ。
  static double reachOf(int player, double p0, double p1) {
    return player == 0 ? p0 : p1;
  }

  // 戦略で重み付けした期待利得（strategy と util の内積）。
  static double expectedUtility(const std::array<double, N>& strategy,
                                const std::array<double, N>& util) {
    double sum = 0.0;
    for (int a = 0; a < N; ++a) {
      sum += strategy[a] * util[a];
    }
    return sum;
  }

  // counterfactual regret を積算する（相手の到達確率で重み付け）。
  static void accumulateRegrets(InformationSet<N>& node,
                                const std::array<double, N>& util,
                                double nodeUtil, double counterfactualReach) {
    for (int a = 0; a < N; ++a) {
      const double regret = util[a] - nodeUtil;
      node.accumulateRegret(a, counterfactualReach * regret);
    }
  }

  G game_;
  // 情報集合のキー → ノード。map で順序付き（出力が読みやすい）。
  std::map<std::string, InformationSet<N>> nodeMap_;
};
}  // namespace cfr
