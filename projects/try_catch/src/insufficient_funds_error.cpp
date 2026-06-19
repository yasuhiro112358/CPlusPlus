#include "insufficient_funds_error.h"
#include <string>

using namespace try_catch;

namespace {
  std::string buildMessage(int balance, int requested) {
    return "残高不足: 残高 " + std::to_string(balance) + " に対して " +
           std::to_string(requested) + " の出金要求";
  }
} // namespace

InsufficientFundsError::InsufficientFundsError(int balance, int requested)
    : std::runtime_error(buildMessage(balance, requested)),
      balance_(balance),
      requested_(requested) {}
