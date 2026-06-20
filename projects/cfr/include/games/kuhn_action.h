#pragma once
#include <cstdint>

namespace cfr {
// Kuhn poker の行動。値は CFR の行動インデックスそのもの。
// kPass = check/fold, kBet = bet/call
enum class KuhnAction : std::uint8_t { kPass = 0, kBet = 1 };
}  // namespace cfr
