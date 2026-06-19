# 静的解析を実行する（clang-tidy）

[clang-tidy](https://clang.llvm.org/extra/clang-tidy/) は、コードを実行せずにバグや
非モダンな書き方を指摘する静的解析ツール。このリポジトリは
[C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/) ベースの設定
（[.clang-tidy](../../.clang-tidy)）で運用する。

## クイックスタート

ラッパースクリプト [scripts/tidy.sh](../../scripts/tidy.sh) を使うのが最も簡単。

```bash
./scripts/tidy.sh                      # 全ソースを解析
./scripts/tidy.sh projects/try_catch/src/bank_account.cpp   # 指定ファイル
./scripts/tidy.sh -fix projects/try_catch/src/bank_account.cpp  # 自動修正
```

このスクリプトは、

- Homebrew の clang-tidy（PATH 非搭載）へのフォールバック
- macOS で標準ライブラリ(SDK)を見つけるための `-isysroot` 補完
- `compile_commands.json`（`-p build`）の指定

をまとめて面倒見てくれる。

## 前提

### 1. clang-tidy のインストール

clang-tidy は LLVM 一式に含まれる。

```bash
brew install llvm
```

Homebrew の LLVM は keg-only（PATH に通らない）。ラッパースクリプトは
`/opt/homebrew/opt/llvm/bin/clang-tidy` を自動で探すが、直接使うなら PATH を通す：

```bash
export PATH="/opt/homebrew/opt/llvm/bin:$PATH"   # ~/.zshrc に追記
```

### 2. compile_commands.json

clang-tidy は各ファイルのコンパイル方法（インクルードパス・C++規格など）を
`compile_commands.json` から読む。トップレベル CMakeLists.txt で
`CMAKE_EXPORT_COMPILE_COMMANDS ON` を有効にしているので、Configure すれば
`build/` に生成される。

```bash
cmake -S . -B build   # build/compile_commands.json が生成される
```

## スクリプトを使わず直接実行する場合

Dev Container（Linux）など SDK 補完が不要な環境ではそのまま実行できる。

```bash
clang-tidy -p build projects/try_catch/src/bank_account.cpp
```

macOS で直接実行するなら `-isysroot` が必要：

```bash
clang-tidy -p build --extra-arg=-isysroot --extra-arg="$(xcrun --show-sdk-path)" \
  projects/try_catch/src/bank_account.cpp
```

## 設定の調整

有効/無効にするチェックは [.clang-tidy](../../.clang-tidy) の `Checks:` で管理する。
無効化（先頭に `-`）は最終手段。**チェックに意味があるなら、コードを直して合わせる**のが
このリポジトリの方針。

```yaml
Checks: >
  modernize-*,
  bugprone-*,
  -readability-magic-numbers   # 無効化の例（練習コードの定数を許容）
```

無効化しているのは、練習コードでノイズになる純粋にスタイル寄りのチェック
（`readability-magic-numbers`、`bugprone-easily-swappable-parameters` 等）のみ。
設計上の指摘（例：状態を持たない演算は自由関数にすべき、main は例外を漏らすべきでない）
には、無効化ではなくコードを直して対応している。

## 関連

- なぜ clang-tidy を導入するのか（モダンC++の指針） → [explanation/build-system-and-environment.md](../explanation/build-system-and-environment.md)
- 設定の各チェック群 → [reference/cmake-and-structure.md](../reference/cmake-and-structure.md)
- clang-format（見た目の整形）とは役割が別。clang-format は整形、clang-tidy はバグ・設計の指摘
