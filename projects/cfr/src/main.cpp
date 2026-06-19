#include "information_set.h"
#include <array>
#include <exception>
#include <iostream>

namespace {
  // 一時的な動作確認用。次の手順で KuhnTrainer の駆動に置き換える。
  void printStrategy(const char* label, const std::array<double, cfr::NUM_ACTIONS>& s) {
    std::cout << label << " pass=" << s[0] << " bet=" << s[1] << '\n';
  }
} // namespace

int main() {
  try {
    cfr::InformationSet node;

    // 後悔がまだ無い → 一様分布（pass 0.5, bet 0.5）になるはず
    printStrategy("初期    :", node.getStrategy(1.0));

    // bet(行動1) への後悔を足す → 次の戦略は bet に偏るはず
    node.accumulateRegret(1, 3.0);
    printStrategy("bet後悔+3:", node.getStrategy(1.0));

    // さらに pass(行動0) にも少し後悔を足す → pass と bet の比に応じた配分になるはず
    node.accumulateRegret(0, 1.0);
    printStrategy("pass後悔+1:", node.getStrategy(1.0));

    return 0;
  } catch (const std::exception& e) {
    std::cerr << "予期しない例外が main に到達: " << e.what() << '\n';
    return 1;
  } catch (...) {
    std::cerr << "予期しない非標準例外が main に到達" << '\n';
    return 1;
  }
}
