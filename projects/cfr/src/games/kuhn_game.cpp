#include "games/kuhn_game.h"

#include <gsl/util>
#include <string>

namespace cfr {
namespace {
// 行動を、履歴文字列に積む文字へ変換する（Pass→'p', Bet→'b'）。
char toHistoryChar(KuhnGame::Action action) {
  // どのケースにも一致しなければ（論理上ありえない）
  char historyChar = '?';
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

std::vector<KuhnGame::State> KuhnGame::initialStates() const {
  return {{.cards = {Card::Jack, Card::Queen}, .history = ""},
          {.cards = {Card::Jack, Card::King}, .history = ""},
          {.cards = {Card::Queen, Card::Jack}, .history = ""},
          {.cards = {Card::Queen, Card::King}, .history = ""},
          {.cards = {Card::King, Card::Jack}, .history = ""},
          {.cards = {Card::King, Card::Queen}, .history = ""}};
}

int KuhnGame::currentPlayer(const State& state) const {
  return static_cast<int>(state.history.size()) % 2;
}

bool KuhnGame::isTerminal(const State& state) const {
  const int plays = static_cast<int>(state.history.size());
  if (plays <= 1) {
    return false;  // 最初の1手だけでは終端にならない
  }
  if (state.history.back() == 'p') {
    return true;  // pp(check-check) / bp(fold) / pbp(fold) は終端
  }
  // 末尾2つが bb（bet-call）なら終端
  return gsl::at(state.history, plays - 2) == 'b';
}

double KuhnGame::utility(const State& state) const {
  const int player = currentPlayer(state);
  const int opponent = 1 - player;
  // Card は値の順が強さの順なので、> がそのまま「自分の方が強い」を意味する。
  const bool playerCardHigher =
      gsl::at(state.cards, player) > gsl::at(state.cards, opponent);

  // bet = ante なので、純益はアンティ単位で +1（無 bet）か +2（bet-call）。
  const double winWithoutBet = ante_;     // check-check / fold（ポット2）
  const double winAfterCall = 2 * ante_;  // bet-call のショーダウン（ポット4）

  if (state.history.back() == 'p') {  // 直前が pass
    if (state.history == "pp") {      // check-check のショーダウン
      return playerCardHigher ? winWithoutBet : -winWithoutBet;
    }
    return winWithoutBet;  // fold → 手番プレイヤーがポットを取る
  }
  // bet-call のショーダウン
  return playerCardHigher ? winAfterCall : -winAfterCall;
}

KuhnGame::State KuhnGame::nextState(const State& state, int action) const {
  return {
      .cards = state.cards,
      .history = state.history + toHistoryChar(static_cast<Action>(action))};
}

std::string KuhnGame::infoSetKey(const State& state) const {
  const int card = static_cast<int>(gsl::at(state.cards, currentPlayer(state)));
  return std::to_string(card) + state.history;
}
}  // namespace cfr
