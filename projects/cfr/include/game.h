#pragma once
#include <concepts>
#include <string>
#include <vector>

namespace cfr {
  // CFR エンジンが「ゲーム」に要求する契約（コンパイル時インターフェース）。
  // std::sort が比較関数を要求するのと同じ発想で、この要件を満たす型なら
  // CfrSolver
  // がそのゲームを解ける。仮想関数を使わないので実行時オーバーヘッドが無い。
  template <typename G>
  concept Game = requires(const G game, const G::State state, int action) {
    // 状態（ゲーム木のノード）。ゲームごとに自由に定義する。
    typename G::State;
    // この状態で選べる行動数。情報集合の配列サイズに使うため定数。
    { G::NUM_ACTIONS } -> std::convertible_to<int>;
    // 探索の起点となる全状態（chance node を列挙：Kuhn は配牌6通り）。
    {
      game.initialStates()
    } -> std::convertible_to<std::vector<typename G::State>>;
    // 手番のプレイヤー（0 or 1）。
    { game.currentPlayer(state) } -> std::convertible_to<int>;
    // 終端か。
    { game.isTerminal(state) } -> std::convertible_to<bool>;
    // 終端での利得（手番プレイヤー視点）。isTerminal が真のときだけ意味を持つ。
    { game.utility(state) } -> std::convertible_to<double>;
    // action を適用した次状態。
    { game.nextState(state, action) } -> std::convertible_to<typename G::State>;
    // 手番プレイヤーの情報集合キー。
    { game.infoSetKey(state) } -> std::convertible_to<std::string>;
  };
} // namespace cfr
