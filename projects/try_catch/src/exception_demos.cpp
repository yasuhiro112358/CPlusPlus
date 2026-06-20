#include "exception_demos.h"

#include <iostream>
#include <stdexcept>
#include <vector>

#include "bank_account.h"
#include "insufficient_funds_error.h"

namespace try_catch {
namespace {
// 1. 基本：標準ライブラリが投げる例外を捕まえる
void DemoStandardException() {
  std::cout << "--- 1. 標準例外 (std::out_of_range) ---" << '\n';
  std::vector<int> v{1, 2, 3};
  const int out_of_range_index = 10;  // size 3 に対して範囲外
  try {
    int x = v.at(out_of_range_index);  // 範囲外アクセス → std::out_of_range
    std::cout << x << '\n';
  } catch (const std::out_of_range& e) {
    // 例外オブジェクトは参照で受けるのが基本（スライシングを避ける）
    std::cout << "捕捉: " << e.what() << '\n';
  }
}

// 2. カスタム例外：自作の例外クラスを投げて捕まえる
void DemoCustomException() {
  std::cout << "\n--- 2. カスタム例外 (InsufficientFundsError) ---" << '\n';
  const int initial_balance = 100;
  const int withdrawal = 150;  // 残高超過 → InsufficientFundsError
  BankAccount account(initial_balance);
  try {
    account.Withdraw(withdrawal);
  } catch (const InsufficientFundsError& e) {
    std::cout << "捕捉: " << e.what() << '\n';
    std::cout << "  残高=" << e.Balance() << " 要求=" << e.Requested() << '\n';
  }
}

// 3. 複数 catch：型ごとに分岐。基底クラスで「その他」を受ける
void DemoMultipleCatch() {
  std::cout << "\n--- 3. 複数 catch ---" << '\n';
  const int initial_balance = 100;
  const int invalid_deposit = -10;  // 負の額 → std::invalid_argument
  BankAccount account(initial_balance);
  try {
    account.Deposit(invalid_deposit);
  } catch (const InsufficientFundsError& e) {
    std::cout << "残高不足: " << e.what() << '\n';
  } catch (const std::exception& e) {
    // InsufficientFundsError 以外の std 例外はここで捕まる（基底クラス捕捉）。
    // 派生クラスの catch を先に、基底クラスの catch を後に書くのが鉄則。
    std::cout << "その他の標準例外: " << e.what() << '\n';
  }
}

// 4. catch(...)：型を問わず全部捕まえる最後の砦
void DemoCatchAll() {
  std::cout << "\n--- 4. catch(...) 全捕捉 ---" << '\n';
  const int non_exception_value = 42;  // std::exception でない任意の値
  try {
    throw non_exception_value;  // int を投げる（std::exception ではない）
  } catch (const std::exception& e) {
    std::cout << "std例外: " << e.what() << '\n';
  } catch (...) {
    // std::exception を継承しない型（int など）はここでしか捕まえられない
    std::cout << "捕捉: std::exception 以外の何か" << '\n';
  }
}

// 5. 例外の伝播：呼び出し先で投げた例外は、捕まえる場所まで遡る
void Risky() {
  const int initial_balance = 50;
  // 残高超過。ここで投げる。risky() は catch しない
  const int withdrawal = 100;
  BankAccount account(initial_balance);
  account.Withdraw(withdrawal);
}

void DemoPropagation() {
  std::cout << "\n--- 5. 例外の伝播 ---" << '\n';
  try {
    Risky();  // risky() 内で投げた例外がここまで伝播してくる
  } catch (const std::exception& e) {
    std::cout << "呼び出し元で捕捉: " << e.what() << '\n';
  }
}
}  // namespace

void RunAllDemos() {
  DemoStandardException();
  DemoCustomException();
  DemoMultipleCatch();
  DemoCatchAll();
  DemoPropagation();
}
}  // namespace try_catch
