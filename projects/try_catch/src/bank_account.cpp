#include "bank_account.h"

#include <stdexcept>

#include "insufficient_funds_error.h"

namespace try_catch {
BankAccount::BankAccount(int initialBalance) : balance_(initialBalance) {}

void BankAccount::deposit(int amount) {
  if (amount < 0) {
    throw std::invalid_argument("入金額が負です");
  }
  balance_ += amount;
}

void BankAccount::withdraw(int amount) {
  if (amount < 0) {
    throw std::invalid_argument("出金額が負です");
  }
  if (amount > balance_) {
    throw InsufficientFundsError(balance_, amount);
  }
  balance_ -= amount;
}
}  // namespace try_catch
