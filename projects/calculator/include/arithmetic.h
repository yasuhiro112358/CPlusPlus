#pragma once

namespace calculator {
// 状態を持たない演算は、クラスにせず自由関数にする（Core Guidelines C.4/C.5）。
int Add(int a, int b);
int Subtract(int a, int b);
}  // namespace calculator
