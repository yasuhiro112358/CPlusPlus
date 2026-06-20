#include "games/kuhn_game.h"
#include <string>

namespace cfr {
  std::vector<KuhnGame::State> KuhnGame::initialStates() const {
    return {{.cards = {0, 1}, .history = ""}, {.cards = {0, 2}, .history = ""},
            {.cards = {1, 0}, .history = ""}, {.cards = {1, 2}, .history = ""},
            {.cards = {2, 0}, .history = ""}, {.cards = {2, 1}, .history = ""}};
  }

  int KuhnGame::currentPlayer(const State& state) const {
    return static_cast<int>(state.history.size()) % 2;
  }

  bool KuhnGame::isTerminal(const State& state) const {
    const int plays = static_cast<int>(state.history.size());
    if (plays <= 1) {
      return false; // 最初の1手だけでは終端にならない
    }
    if (state.history.back() == 'p') {
      return true; // pp(check-check) / bp(fold) / pbp(fold) は終端
    }
    return state.history[plays - 2] == 'b'; // 末尾2つが bb（bet-call）なら終端
  }

  double KuhnGame::utility(const State& state) const {
    const int player = currentPlayer(state);
    const int opponent = 1 - player;
    const bool playerCardHigher = state.cards[player] > state.cards[opponent];

    if (state.history.back() == 'p') { // 直前が pass
      if (state.history == "pp") {
        return playerCardHigher ? 1.0
                                : -1.0; // check-check のショーダウン（ポット2）
      }
      return 1.0; // fold → 手番プレイヤーがポットを取る
    }
    return playerCardHigher ? 2.0 : -2.0; // bet-call のショーダウン（ポット4）
  }

  KuhnGame::State KuhnGame::nextState(const State& state, int action) const {
    return {.cards = state.cards,
            .history = state.history + (action == 0 ? 'p' : 'b')};
  }

  std::string KuhnGame::infoSetKey(const State& state) const {
    return std::to_string(state.cards[currentPlayer(state)]) + state.history;
  }
} // namespace cfr
