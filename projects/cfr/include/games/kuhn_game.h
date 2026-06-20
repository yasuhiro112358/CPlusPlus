#pragma once
#include <cstdint>
#include <vector>

namespace cfr {
class KuhnState;  // 局面（kuhn_state.h で定義）。相互依存のため前方宣言。

// Kuhn poker のルールと設定（アンティ額）。局面そのものは KuhnState が表す。
class KuhnGame {
 public:
  // 行動。基となる整数(0,1)が CFR エンジンの行動インデックスに一致する。
  // Pass=check/fold, Bet=bet/call
  enum class Action : std::uint8_t { Pass, Bet };
  static constexpr int NUM_ACTIONS = 2;

  // カード。値の順序がそのまま強さの順（Jack < Queen < King）。
  enum class Card : std::uint8_t { Jack, Queen, King };

  using State = KuhnState;

  // アンティの絶対額を渡して構成する。Kuhn の定義により bet 額は ante と同額。
  // 利得はこの ante を単位に計算する。デフォルト 1.0 が正準 Kuhn。
  explicit KuhnGame(double ante = 1.0) : ante_(ante) {}

  // 1人あたりのアンティ額（利得の単位）。State が利得計算に使う。
  [[nodiscard]] double ante() const { return ante_; }

  // 探索の起点：3枚から2人へ配る全6通り。
  // chance node を全列挙する vanilla CFR。
  [[nodiscard]] std::vector<KuhnState> initialStates() const;

 private:
  double ante_;
};
}  // namespace cfr
