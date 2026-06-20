#pragma once
#include <concepts>
#include <string>
#include <vector>

namespace cfr {
// CFR エンジンが「ゲーム」に要求する契約（コンパイル時インターフェース）。
// std::sort が比較関数を要求するのと同じ発想。
// この要件を満たす型なら CfrSolver が解ける。
// 仮想関数を使わないので実行時オーバーヘッドが無い。
//
// ゲームは「ルール＋設定」、局面(State)は「ゲーム木のノード」。
// 各ノードの問い合わせ（手番・終端・利得・次局面・キー）は State 自身が答える。
template <typename G>
concept Game = requires(const G game, const G::State state, int action) {
  // 局面（ゲーム木のノード）の型。
  typename G::State;
  // 行動数。情報集合の配列サイズに使うため定数。
  { G::NUM_ACTIONS } -> std::convertible_to<int>;
  // 探索の起点となる全局面（chance node を列挙：Kuhn は配牌6通り）。
  {
    game.initialStates()
  } -> std::convertible_to<std::vector<typename G::State>>;
  // 以下は局面(State)自身が答える。
  { state.currentPlayer() } -> std::convertible_to<int>;  // 手番（0/1）
  { state.isTerminal() } -> std::convertible_to<bool>;    // 終端か
  { state.utility() } -> std::convertible_to<double>;     // 終端利得
  {
    state.next(action)
  } -> std::convertible_to<typename G::State>;                 // 1手後の局面
  { state.infoSetKey() } -> std::convertible_to<std::string>;  // 情報集合キー
};
}  // namespace cfr
