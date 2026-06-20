#include "games/kuhn_game.h"

#include "games/kuhn_state.h"

namespace cfr {
std::vector<KuhnState> KuhnGame::initialStates() const {
  return {KuhnState(this, {Card::Jack, Card::Queen}, ""),
          KuhnState(this, {Card::Jack, Card::King}, ""),
          KuhnState(this, {Card::Queen, Card::Jack}, ""),
          KuhnState(this, {Card::Queen, Card::King}, ""),
          KuhnState(this, {Card::King, Card::Jack}, ""),
          KuhnState(this, {Card::King, Card::Queen}, "")};
}
}  // namespace cfr
