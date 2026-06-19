#include "kuhn_trainer.h"
#include <array>
#include <iostream>
#include <string>

namespace cfr {
  double KuhnTrainer::cfr(const std::array<int, 2>& cards, const std::string& history, double p0,
                          double p1) {
    const int plays = static_cast<int>(history.size());
    const int player = plays % 2;
    const int opponent = 1 - player;

    // 1. 終端なら利得を返す（手番プレイヤー視点）
    if (plays > 1) {
      const bool terminalPass = history.back() == 'p';
      const bool doubleBet = history[plays - 2] == 'b' && history[plays - 1] == 'b';
      const bool playerCardHigher = cards[player] > cards[opponent];
      if (terminalPass) {
        if (history == "pp") {
          return playerCardHigher ? 1.0 : -1.0; // check-check のショーダウン（ポット2）
        }
        return 1.0; // 直前が fold → 手番プレイヤーがポットを取る
      }
      if (doubleBet) {
        return playerCardHigher ? 2.0 : -2.0; // bet-call のショーダウン（ポット4）
      }
    }

    // 2. この情報集合のノードを取得（無ければ作られる）
    const std::string infoSet = std::to_string(cards[player]) + history;
    InformationSet& node = nodeMap_[infoSet];

    // 3. 各行動の利得を再帰で求める（子は相手視点なので符号反転する）
    const double realizationWeight = (player == 0) ? p0 : p1;
    const std::array<double, NUM_ACTIONS> strategy = node.getStrategy(realizationWeight);
    std::array<double, NUM_ACTIONS> util{};
    double nodeUtil = 0.0;
    for (int a = 0; a < NUM_ACTIONS; ++a) {
      const std::string nextHistory = history + (a == 0 ? 'p' : 'b');
      if (player == 0) {
        util[a] = -cfr(cards, nextHistory, p0 * strategy[a], p1);
      } else {
        util[a] = -cfr(cards, nextHistory, p0, p1 * strategy[a]);
      }
      nodeUtil += strategy[a] * util[a];
    }

    // 4. counterfactual regret を積算する（相手の到達確率で重み付け）
    const double counterfactualReach = (player == 0) ? p1 : p0;
    for (int a = 0; a < NUM_ACTIONS; ++a) {
      const double regret = util[a] - nodeUtil;
      node.accumulateRegret(a, counterfactualReach * regret);
    }
    return nodeUtil;
  }

  double KuhnTrainer::train(int iterations) {
    // 3枚から2人に配る全6通り（chance node を全列挙する vanilla CFR）
    static constexpr std::array<std::array<int, 2>, 6> DEALS = {
        {{0, 1}, {0, 2}, {1, 0}, {1, 2}, {2, 0}, {2, 1}}};
    double utilSum = 0.0;
    for (int i = 0; i < iterations; ++i) {
      for (const auto& cards : DEALS) {
        utilSum += cfr(cards, "", 1.0, 1.0);
      }
    }
    return utilSum / (iterations * static_cast<double>(DEALS.size()));
  }

  void KuhnTrainer::printStrategy() const {
    std::cout << "--- 平均戦略（情報集合: pass/bet）---\n";
    for (const auto& [infoSet, node] : nodeMap_) {
      const std::array<double, NUM_ACTIONS> avg = node.getAverageStrategy();
      std::cout << infoSet << " : pass=" << avg[0] << " bet=" << avg[1] << '\n';
    }
  }
} // namespace cfr
