#pragma once
#include <algorithm>
#include <array>
#include <gsl/util>

namespace cfr {
// 1つの情報集合の後悔(regret)と戦略を積算し、regret matching で戦略を作る。
// NumActions はゲームごとに決まる行動数（テンプレート引数）。
template <int NumActions>
class InformationSet {
 public:
  // regret matching で現在の戦略を計算する。
  // realizationWeight（この情報集合への到達確率）で平均戦略用に積算もする。
  std::array<double, NumActions> getStrategy(double realizationWeight) {
    // 1. 正の後悔だけを取り出す（後悔が大きい行動ほど選びたい）
    std::array<double, NumActions> strategy{};
    double normalizingSum = 0.0;
    for (int a = 0; a < NumActions; ++a) {
      gsl::at(strategy, a) = std::max(gsl::at(regretSum_, a), 0.0);  // 負は0に
      normalizingSum += gsl::at(strategy, a);
    }

    // 2. 正規化（合計1に）。後悔が全く無ければ一様分布にする
    for (int a = 0; a < NumActions; ++a) {
      if (normalizingSum > 0.0) {
        gsl::at(strategy, a) /= normalizingSum;
      } else {
        gsl::at(strategy, a) = 1.0 / NumActions;
      }
      // 3. 平均戦略のために、到達確率で重み付けして積算する
      gsl::at(strategySum_, a) += realizationWeight * gsl::at(strategy, a);
    }
    return strategy;
  }

  // ある行動への counterfactual regret を足し込む。
  void accumulateRegret(int action, double regret) {
    gsl::at(regretSum_, action) += regret;
  }

  // 収束後の平均戦略（GTO）を返す。
  [[nodiscard]] std::array<double, NumActions> getAverageStrategy() const {
    std::array<double, NumActions> average{};
    double normalizingSum = 0.0;
    for (int a = 0; a < NumActions; ++a) {
      normalizingSum += gsl::at(strategySum_, a);
    }
    for (int a = 0; a < NumActions; ++a) {
      if (normalizingSum > 0.0) {
        gsl::at(average, a) = gsl::at(strategySum_, a) / normalizingSum;
      } else {
        gsl::at(average, a) = 1.0 / NumActions;  // 未学習なら一様分布
      }
    }
    return average;
  }

 private:
  std::array<double, NumActions> regretSum_{};    // 各行動への累積後悔
  std::array<double, NumActions> strategySum_{};  // 平均戦略を出すための累積
};
}  // namespace cfr
