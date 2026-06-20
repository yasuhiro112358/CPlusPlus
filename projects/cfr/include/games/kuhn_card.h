#pragma once
#include <cstdint>

namespace cfr {
// Kuhn poker のカード。値は実ポーカーのランク（J=11, Q=12, K=13）で、
// 順序がそのまま強さの順。整数値は InfoSetKey の出力に現れる。
enum class KuhnCard : std::uint8_t { kJack = 11, kQueen = 12, kKing = 13 };
}  // namespace cfr
