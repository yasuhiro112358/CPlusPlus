#pragma once
#include <string>

namespace calculator {
  class App {
  private:
    static std::string appName_;

  public:
    App();
    void run();
  };
} // namespace calculator
