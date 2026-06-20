#include "games/kuhn_state.h"

#include <gsl/util>
#include <string>
#include <utility>

namespace cfr {
namespace {
// 行動を、履歴文字列に積む文字へ変換する（Pass→'p', Bet→'b'）。
char toHistoryChar(KuhnGame::Action action) {
  char historyChar = '?';  // どのケースにも一致しなければ（論理上ありえない）
  switch (action) {
    case KuhnGame::Action::Pass:
      historyChar = 'p';
      break;
    case KuhnGame::Action::Bet:
      historyChar = 'b';
      break;
  }
  return historyChar;
}
}  // namespace

KuhnState::KuhnState(const KuhnGame* game, std::array<KuhnGame::Card, 2> cards,
                     std::string history)
    : game_(game), cards_(cards), history_(std::move(history)) {}

int KuhnState::currentPlayer() const {
  return static_cast<int>(history_.size()) % 2;
}

bool KuhnState::isTerminal() const {
  const int plays = static_cast<int>(history_.size());
  if (plays <= 1) {
    return false;  // 最初の1手だけでは終端にならない
  }
  if (history_.back() == 'p') {
    return true;  // pp(check-check) / bp(fold) / pbp(fold) は終端
  }
  // 末尾2つが bb（bet-call）なら終端
  return gsl::at(history_, plays - 2) == 'b';
}

double KuhnState::utility() const {
  const int player = currentPlayer();
  const int opponent = 1 - player;
  // Card は値の順が強さの順なので、> がそのまま「自分の方が強い」を意味する。
  const bool playerCardHigher =
      gsl::at(cards_, player) > gsl::at(cards_, opponent);

  // bet = ante なので、純益はアンティ単位で +1（無 bet）か +2（bet-call）。
  const double ante = game_->ante();
  const double winWithoutBet = ante;     // check-check / fold（ポット2）
  const double winAfterCall = 2 * ante;  // bet-call のショーダウン（ポット4）

  if (history_.back() == 'p') {  // 直前が pass
    if (history_ == "pp") {      // check-check のショーダウン
      return playerCardHigher ? winWithoutBet : -winWithoutBet;
    }
    return winWithoutBet;  // fold → 手番プレイヤーがポットを取る
  }
  // bet-call のショーダウン
  return playerCardHigher ? winAfterCall : -winAfterCall;
}

KuhnState KuhnState::next(int action) const {
  return {game_, cards_,
          history_ + toHistoryChar(static_cast<KuhnGame::Action>(action))};
}

std::string KuhnState::infoSetKey() const {
  const int card = static_cast<int>(gsl::at(cards_, currentPlayer()));
  return std::to_string(card) + history_;
}
}  // namespace cfr
