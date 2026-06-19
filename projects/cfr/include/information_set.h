#pragma once
#include <array>

namespace cfr {
  // Kuhn poker の行動は2つ：pass(check/fold) と bet(bet/call)
  inline constexpr int NUM_ACTIONS = 2;

  // 1つの情報集合の後悔(regret)と戦略を積算し、regret matching で戦略を作る。
  class InformationSet {
  public:
    // regret matching で現在の戦略を計算する。
    // realizationWeight（この情報集合への到達確率）で平均戦略用に積算もする。
    std::array<double, NUM_ACTIONS> getStrategy(double realizationWeight);

    // ある行動への counterfactual regret を足し込む。
    void accumulateRegret(int action, double regret);

    // 収束後の平均戦略（GTO）を返す。
    [[nodiscard]] std::array<double, NUM_ACTIONS> getAverageStrategy() const;

  private:
    std::array<double, NUM_ACTIONS> regretSum_{};   // 各行動への累積後悔
    std::array<double, NUM_ACTIONS> strategySum_{}; // 平均戦略を出すための累積
  };
} // namespace cfr
