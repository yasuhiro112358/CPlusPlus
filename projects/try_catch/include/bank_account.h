#pragma once

namespace try_catch {
class BankAccount {
 public:
  explicit BankAccount(int initial_balance = 0);

  // 負の額なら std::invalid_argument
  void Deposit(int amount);
  // 負の額なら std::invalid_argument、残高不足なら InsufficientFundsError
  void Withdraw(int amount);
  [[nodiscard]] int Balance() const { return balance_; }

 private:
  int balance_;
};
}  // namespace try_catch
