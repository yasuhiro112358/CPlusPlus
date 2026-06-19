#pragma once
#include "information_set.h"
#include <array>
#include <map>
#include <string>

namespace cfr {
  // Kuhn poker を CFR で学習するトレーナー。
  // 情報集合のテーブルを持ち、ゲーム木を辿る cfr() を反復して GTO に近づける。
  class KuhnTrainer {
  public:
    // iterations 回学習し、プレイヤー0視点の平均ゲーム価値を返す。
    double train(int iterations);

    // 学習後、各情報集合の平均戦略（GTO）を表示する。
    void printStrategy() const;

  private:
    // ゲーム木を辿り、手番プレイヤー視点の利得を返す。
    // cards: 2人に配られたカード, history: これまでの行動("p"/"b"の列), p0/p1: 各人の到達確率。
    double cfr(const std::array<int, 2>& cards, const std::string& history, double p0, double p1);

    // 情報集合のキー("カード+履歴") → ノード。map で順序付き（出力が読みやすい）。
    std::map<std::string, InformationSet> nodeMap_;
  };
} // namespace cfr
