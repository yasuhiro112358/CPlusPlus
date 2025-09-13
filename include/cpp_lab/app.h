#pragma once
#include <string>

namespace cpp_lab {
  class App {
  private:
    static std::string appName_;

  public:
    // Constructor
    App();

    // Run the application
    void run();
  };
} // namespace cpp_lab
