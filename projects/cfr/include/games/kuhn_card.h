#pragma once
#include <cstdint>

namespace cfr {
// Kuhn poker のカード。値の順序がそのまま強さの順（Jack < Queen < King）。
enum class KuhnCard : std::uint8_t { kJack, kQueen, kKing };
}  // namespace cfr
