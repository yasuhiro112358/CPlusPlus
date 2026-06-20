#include "games/kuhn_game.h"

#include "games/kuhn_state.h"

namespace cfr {
std::vector<KuhnState> KuhnGame::InitialStates() const {
  return {KuhnState(this, {Card::kJack, Card::kQueen}, ""),
          KuhnState(this, {Card::kJack, Card::kKing}, ""),
          KuhnState(this, {Card::kQueen, Card::kJack}, ""),
          KuhnState(this, {Card::kQueen, Card::kKing}, ""),
          KuhnState(this, {Card::kKing, Card::kJack}, ""),
          KuhnState(this, {Card::kKing, Card::kQueen}, "")};
}
}  // namespace cfr
