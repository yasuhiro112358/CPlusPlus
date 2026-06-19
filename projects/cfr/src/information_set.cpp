#include "information_set.h"
#include <algorithm>

namespace cfr {
  std::array<double, NUM_ACTIONS> InformationSet::getStrategy(double realizationWeight) {
    // 1. 正の後悔だけを取り出す（後悔が大きい行動ほど選びたい）
    std::array<double, NUM_ACTIONS> strategy{};
    double normalizingSum = 0.0;
    for (int a = 0; a < NUM_ACTIONS; ++a) {
      strategy[a] = std::max(regretSum_[a], 0.0); // 負の後悔は0にクリップ
      normalizingSum += strategy[a];
    }

    // 2. 正規化（合計1に）。後悔が全く無ければ一様分布にする
    for (int a = 0; a < NUM_ACTIONS; ++a) {
      if (normalizingSum > 0.0) {
        strategy[a] /= normalizingSum;
      } else {
        strategy[a] = 1.0 / NUM_ACTIONS;
      }
      // 3. 平均戦略のために、到達確率で重み付けして積算する
      strategySum_[a] += realizationWeight * strategy[a];
    }
    return strategy;
  }

  void InformationSet::accumulateRegret(int action, double regret) {
    regretSum_[action] += regret;
  }

  std::array<double, NUM_ACTIONS> InformationSet::getAverageStrategy() const {
    std::array<double, NUM_ACTIONS> average{};
    double normalizingSum = 0.0;
    for (int a = 0; a < NUM_ACTIONS; ++a) {
      normalizingSum += strategySum_[a];
    }
    for (int a = 0; a < NUM_ACTIONS; ++a) {
      if (normalizingSum > 0.0) {
        average[a] = strategySum_[a] / normalizingSum;
      } else {
        average[a] = 1.0 / NUM_ACTIONS; // まだ何も学習していない場合は一様分布
      }
    }
    return average;
  }
} // namespace cfr
