#pragma once

namespace try_catch {
  class BankAccount {
  public:
    explicit BankAccount(int initialBalance = 0);

    void deposit(int amount);  // 負の額なら std::invalid_argument
    void withdraw(int amount); // 負の額なら std::invalid_argument、残高不足なら InsufficientFundsError
    int balance() const { return balance_; }

  private:
    int balance_;
  };
} // namespace try_catch
