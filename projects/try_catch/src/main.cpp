#include "bank_account.h"
#include "insufficient_funds_error.h"
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace try_catch;

// 1. 基本：標準ライブラリが投げる例外を捕まえる
void demoStandardException() {
  std::cout << "--- 1. 標準例外 (std::out_of_range) ---" << std::endl;
  std::vector<int> v{1, 2, 3};
  try {
    int x = v.at(10); // 範囲外アクセス → std::out_of_range を投げる
    std::cout << x << std::endl;
  } catch (const std::out_of_range& e) {
    // 例外オブジェクトは参照で受けるのが基本（コピーを避ける）
    std::cout << "捕捉: " << e.what() << std::endl;
  }
}

// 2. カスタム例外：自作の例外クラスを投げて捕まえる
void demoCustomException() {
  std::cout << "\n--- 2. カスタム例外 (InsufficientFundsError) ---" << std::endl;
  BankAccount account(100);
  try {
    account.withdraw(150);
  } catch (const InsufficientFundsError& e) {
    std::cout << "捕捉: " << e.what() << std::endl;
    std::cout << "  残高=" << e.balance() << " 要求=" << e.requested() << std::endl;
  }
}

// 3. 複数 catch：型ごとに分岐。基底クラスで「その他」を受ける
void demoMultipleCatch() {
  std::cout << "\n--- 3. 複数 catch ---" << std::endl;
  BankAccount account(100);
  try {
    account.deposit(-10); // std::invalid_argument を投げる
  } catch (const InsufficientFundsError& e) {
    std::cout << "残高不足: " << e.what() << std::endl;
  } catch (const std::exception& e) {
    // InsufficientFundsError 以外の std 例外はここで捕まる（基底クラス捕捉）。
    // 派生クラスの catch を先に、基底クラスの catch を後に書くのが鉄則。
    std::cout << "その他の標準例外: " << e.what() << std::endl;
  }
}

// 4. catch(...)：型を問わず全部捕まえる最後の砦
void demoCatchAll() {
  std::cout << "\n--- 4. catch(...) 全捕捉 ---" << std::endl;
  try {
    throw 42; // int を投げる（std::exception ではない）
  } catch (const std::exception& e) {
    std::cout << "std例外: " << e.what() << std::endl;
  } catch (...) {
    // std::exception を継承しない型（int など）はここでしか捕まえられない
    std::cout << "捕捉: std::exception 以外の何か" << std::endl;
  }
}

// 5. 例外の伝播：呼び出し先で投げた例外は、捕まえる場所まで遡る
void risky() {
  BankAccount account(50);
  account.withdraw(100); // ここで投げる。risky() は catch しない
}

void demoPropagation() {
  std::cout << "\n--- 5. 例外の伝播 ---" << std::endl;
  try {
    risky(); // risky() 内で投げた例外がここまで伝播してくる
  } catch (const std::exception& e) {
    std::cout << "呼び出し元で捕捉: " << e.what() << std::endl;
  }
}

int main() {
  demoStandardException();
  demoCustomException();
  demoMultipleCatch();
  demoCatchAll();
  demoPropagation();
  return 0;
}
