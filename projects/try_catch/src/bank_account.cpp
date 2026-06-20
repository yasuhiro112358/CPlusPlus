#include "bank_account.h"

#include <stdexcept>

#include "insufficient_funds_error.h"

namespace try_catch {
BankAccount::BankAccount(int initial_balance) : balance_(initial_balance) {}

void BankAccount::Deposit(int amount) {
  if (amount < 0) {
    throw std::invalid_argument("入金額が負です");
  }
  balance_ += amount;
}

void BankAccount::Withdraw(int amount) {
  if (amount < 0) {
    throw std::invalid_argument("出金額が負です");
  }
  if (amount > balance_) {
    throw InsufficientFundsError(balance_, amount);
  }
  balance_ -= amount;
}
}  // namespace try_catch
