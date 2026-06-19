#pragma once
#include <stdexcept>

namespace try_catch {
  // 残高不足を表すカスタム例外。std::runtime_error を継承する。
  // 標準例外を継承しておくと、catch (const std::exception&) でまとめて捕まえられる。
  class InsufficientFundsError : public std::runtime_error {
  public:
    InsufficientFundsError(int balance, int requested);

    int balance() const { return balance_; }
    int requested() const { return requested_; }

  private:
    int balance_;
    int requested_;
  };
} // namespace try_catch
