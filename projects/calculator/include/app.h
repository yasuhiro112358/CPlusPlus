#pragma once
#include <string>

namespace calculator {
  class App {
  public:
    App();
    void run();

  private:
    static std::string appName_;
  };
} // namespace calculator
