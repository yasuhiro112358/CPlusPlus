#pragma once
#include <array>
#include <string>

#include "games/kuhn_game.h"

namespace cfr {
// Kuhn poker の局面（ゲーム木のノード）。配牌と履歴を持ち、自分自身を語る。
// 利得計算に使う設定（アンティ額）は、所属する KuhnGame を参照する。
class KuhnState {
 public:
  KuhnState(const KuhnGame* game, std::array<KuhnGame::Card, 2> cards,
            std::string history);

  // 手番のプレイヤー（手数の偶奇）。
  [[nodiscard]] int currentPlayer() const;

  // 終端判定（fold で終わるか、bet-call が成立したか）。
  [[nodiscard]] bool isTerminal() const;

  // 終端での利得（手番プレイヤー視点）。isTerminal が真のときだけ呼ぶ。
  [[nodiscard]] double utility() const;

  // action を適用した次の局面（新しい値を返す。元の局面は変えない）。
  [[nodiscard]] KuhnState next(int action) const;

  // 情報集合のキー = 自分のカード + これまでの履歴。
  [[nodiscard]] std::string infoSetKey() const;

 private:
  const KuhnGame* game_;                 // 所属するゲーム（アンティ等の設定）
  std::array<KuhnGame::Card, 2> cards_;  // 2人に配られたカード
  std::string history_;                  // これまでの行動（'p'/'b' の列）
};
}  // namespace cfr
