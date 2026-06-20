#pragma once
#include <vector>

namespace cfr {
class KuhnState;  // 局面（kuhn_state.h で定義）。相互依存のため前方宣言。

// Kuhn poker のルールと設定（アンティ額）。局面そのものは KuhnState が表す。
// カード・行動の語彙は kuhn_card.h / kuhn_action.h に独立している。
class KuhnGame {
 public:
  static constexpr int kNumActions = 2;  // KuhnAction の値数（pass / bet）

  using State = KuhnState;

  // アンティの絶対額を渡して構成する。Kuhn の定義により bet 額は ante と同額。
  // 利得はこの ante を単位に計算する。デフォルト 1.0 が正準 Kuhn。
  explicit KuhnGame(double ante = 1.0) : ante_(ante) {}

  // 1人あたりのアンティ額（利得の単位）。State が利得計算に使う。
  [[nodiscard]] double Ante() const { return ante_; }

  // 探索の起点：3枚から2人へ配る全6通り。
  // chance node を全列挙する vanilla CFR。
  [[nodiscard]] std::vector<KuhnState> InitialStates() const;

 private:
  double ante_;
};
}  // namespace cfr
