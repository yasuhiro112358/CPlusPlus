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
  std::array<double, NumActions> GetStrategy(double realization_weight) {
    // 1. 正の後悔だけを取り出す（後悔が大きい行動ほど選びたい）
    std::array<double, NumActions> strategy{};
    double normalizing_sum = 0.0;
    for (int a = 0; a < NumActions; ++a) {
      gsl::at(strategy, a) = std::max(gsl::at(regret_sum_, a), 0.0);  // 負は0に
      normalizing_sum += gsl::at(strategy, a);
    }

    // 2. 正規化（合計1に）。後悔が全く無ければ一様分布にする
    for (int a = 0; a < NumActions; ++a) {
      if (normalizing_sum > 0.0) {
        gsl::at(strategy, a) /= normalizing_sum;
      } else {
        gsl::at(strategy, a) = 1.0 / NumActions;
      }
      // 3. 平均戦略のために、到達確率で重み付けして積算する
      gsl::at(strategy_sum_, a) += realization_weight * gsl::at(strategy, a);
    }
    return strategy;
  }

  // ある行動への counterfactual regret を足し込む。
  void AccumulateRegret(int action, double regret) {
    gsl::at(regret_sum_, action) += regret;
  }

  // 収束後の平均戦略（GTO）を返す。
  [[nodiscard]] std::array<double, NumActions> GetAverageStrategy() const {
    std::array<double, NumActions> average{};
    double normalizing_sum = 0.0;
    for (int a = 0; a < NumActions; ++a) {
      normalizing_sum += gsl::at(strategy_sum_, a);
    }
    for (int a = 0; a < NumActions; ++a) {
      if (normalizing_sum > 0.0) {
        gsl::at(average, a) = gsl::at(strategy_sum_, a) / normalizing_sum;
      } else {
        gsl::at(average, a) = 1.0 / NumActions;  // 未学習なら一様分布
      }
    }
    return average;
  }

 private:
  std::array<double, NumActions> regret_sum_{};    // 各行動への累積後悔
  std::array<double, NumActions> strategy_sum_{};  // 平均戦略を出すための累積
};
}  // namespace cfr
