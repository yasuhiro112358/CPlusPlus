#pragma once
#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace cfr {
// Kuhn poker のルールを CFR エンジン（Game concept）向けに定義する。
// 状態は「2人の配牌」と「これまでの行動履歴」だけ。
class KuhnGame {
 public:
  // 行動。基となる整数(0,1)が CFR エンジンの行動インデックスに一致する。
  // Pass=check/fold, Bet=bet/call
  enum class Action : std::uint8_t { Pass, Bet };
  static constexpr int NUM_ACTIONS = 2;

  // カード。値の順序がそのまま強さの順（Jack < Queen < King）。
  enum class Card : std::uint8_t { Jack, Queen, King };

  struct State {
    std::array<Card, 2> cards;  // 2人に配られたカード
    std::string history;        // これまでの行動（'p'/'b' の列）
  };

  // 探索の起点：3枚から2人へ配る全6通り。
  // chance node を全列挙する vanilla CFR。
  [[nodiscard]] std::vector<State> initialStates() const;

  // 手番のプレイヤー（手数の偶奇）。
  [[nodiscard]] int currentPlayer(const State& state) const;

  // 終端判定（fold で終わるか、bet-call が成立したか）。
  [[nodiscard]] bool isTerminal(const State& state) const;

  // 終端での利得（手番プレイヤー視点）。isTerminal が真のときだけ呼ぶ。
  [[nodiscard]] double utility(const State& state) const;

  // action を適用した次状態。
  [[nodiscard]] State nextState(const State& state, int action) const;

  // 情報集合のキー = 自分のカード + これまでの履歴。
  [[nodiscard]] std::string infoSetKey(const State& state) const;
};
}  // namespace cfr
