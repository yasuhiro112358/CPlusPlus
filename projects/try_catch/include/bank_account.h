#pragma once

namespace try_catch {
class BankAccount {
 public:
  explicit BankAccount(int initialBalance = 0);

  // 負の額なら std::invalid_argument
  void deposit(int amount);
  // 負の額なら std::invalid_argument、残高不足なら InsufficientFundsError
  void withdraw(int amount);
  [[nodiscard]] int balance() const { return balance_; }

 private:
  int balance_;
};
}  // namespace try_catch
