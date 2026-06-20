#pragma once
#include <cstdint>

namespace cfr {
// Kuhn poker の行動。整数 0/1 が CFR の行動インデックスに対応する。
// kPass = check/fold, kBet = bet/call
enum class KuhnAction : std::uint8_t { kPass, kBet };
}  // namespace cfr
