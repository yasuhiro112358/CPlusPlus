#include "games/kuhn_game.h"

#include "games/kuhn_card.h"
#include "games/kuhn_state.h"

namespace cfr {
std::vector<KuhnState> KuhnGame::InitialStates() const {
  return {KuhnState(this, {KuhnCard::kJack, KuhnCard::kQueen}, ""),
          KuhnState(this, {KuhnCard::kJack, KuhnCard::kKing}, ""),
          KuhnState(this, {KuhnCard::kQueen, KuhnCard::kJack}, ""),
          KuhnState(this, {KuhnCard::kQueen, KuhnCard::kKing}, ""),
          KuhnState(this, {KuhnCard::kKing, KuhnCard::kJack}, ""),
          KuhnState(this, {KuhnCard::kKing, KuhnCard::kQueen}, "")};
}
}  // namespace cfr
