#pragma once
#include <string_view>

namespace calculator {
class App {
 public:
  App();
  void run();

 private:
  // App はこの名前を「インスタンスの状態」として持つ。
  // run() がこのメンバを使うことで、run() は正当なインスタンスメソッドになる。
  std::string_view appName_ = "calculator";
};
}  // namespace calculator
