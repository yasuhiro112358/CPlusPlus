# リファレンス：構成・CMake・ツール

事実を引くためのリファレンス。手順や背景は別ドキュメントを参照。

## ディレクトリ構成

```text
.
├── CMakeLists.txt              # トップレベル。各プロジェクトを束ねる
├── README.md
├── docs/                       # このドキュメント群（Diátaxis）
├── .devcontainer/              # Dev Container 定義
│   ├── devcontainer.json
│   └── dockerfile
├── .vscode/settings.json       # 保存時フォーマット等のエディタ設定
├── .clang-format               # コード整形ルール
├── .gitignore                  # build/ と .DS_Store を除外
└── projects/
    └── calculator/             # テーマごとに独立した CMake プロジェクト
        ├── CMakeLists.txt
        ├── include/            # ヘッダ（.h）
        │   ├── app.h
        │   └── calculator.h
        └── src/                # 実装（.cpp）
            ├── main.cpp
            ├── app.cpp
            └── calculator.cpp
```

## トップレベル CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.20)
project(CPlusPlus LANGUAGES CXX)

add_subdirectory(projects/calculator)
```

| 行 | 意味 |
|----|------|
| `cmake_minimum_required` | 要求する CMake の最低バージョン |
| `project(... LANGUAGES CXX)` | プロジェクト名と使用言語（C++） |
| `add_subdirectory(...)` | サブプロジェクトを取り込む。テーマ追加ごとに1行増やす |

## プロジェクト CMakeLists.txt（calculator）

```cmake
cmake_minimum_required(VERSION 3.20)
project(calculator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/app.cpp
    src/calculator.cpp
)
target_include_directories(${PROJECT_NAME} PRIVATE include)
```

| 設定 | 意味 |
|------|------|
| `CMAKE_CXX_STANDARD 20` | C++20 を使う |
| `CMAKE_CXX_STANDARD_REQUIRED ON` | C++20 が使えなければエラーにする（黙って古い規格に落とさない） |
| `CMAKE_CXX_EXTENSIONS OFF` | GNU拡張を無効化し、標準準拠にする |
| `add_executable` | 実行ファイル名と含めるソースファイル |
| `target_include_directories ... PRIVATE include` | ヘッダの検索パスに `include/` を追加 |

## ビルドコマンド

| コマンド | 役割 |
|---------|------|
| `cmake -S . -B build` | Configure。`-S`=ソース位置、`-B`=出力先 |
| `cmake --build build` | Build。中で `make` が走る |
| `cmake --build build --target <name>` | 特定プロジェクトだけビルド |
| `rm -rf build` | ビルド成果物を全消去（リセット） |

## build/ ディレクトリの生成物

`cmake -S . -B build` で生成される。すべて生成物であり Git 管理しない（`.gitignore` で除外）。

| ファイル/ディレクトリ | 役割 |
|---------------------|------|
| `Makefile` | ビルド手順書。`cmake --build` が `make` に渡す |
| `CMakeCache.txt` | 設定のキャッシュ（コンパイラパス・C++規格など） |
| `cmake_install.cmake` | `make install` 時の配置ルール |
| `CMakeFiles/` | 依存関係追跡などの内部データ |
| `projects/<name>/<name>` | 各プロジェクトの実行ファイル |

## ツールチェーン（検証済みの実例）

| ツール | 役割 | このマシンでの状態 |
|--------|------|------------------|
| `clang++` | C++ コンパイラ | あり（Apple clang 21.0.0、`/usr/bin/clang++`） |
| `cmake` | ビルド構成ツール | あり（4.3.3、`brew install` で導入） |
| `make` | ビルド実行 | あり（Xcode付属） |
| `clangd` | エディタ補完用の言語サーバー | あり（`/usr/bin/clangd`） |
| `clang-format` | コード整形 | 未インストール（`brew install clang-format`） |
| `clang-tidy` | 静的解析 | 未インストール（`brew install llvm`） |

macOS では `gcc`/`g++` も実体は Clang へのエイリアスになっている。

## 命名規則（LLVMベース）

| 対象 | 規則 | 例 |
|------|------|----|
| 型（class/struct/enum） | PascalCase | `Person`, `Vector2` |
| 関数/メソッド | camelCase | `getName`, `setAge` |
| 変数 | camelCase | `userName`, `count` |
| メンバ変数 | camelCase + `_` | `userName_`, `count_` |
| 定数/マクロ | UPPER_CASE | `MAX_SIZE` |
| 名前空間 | snake_case | `cpp_lab::models` |
| ファイル | snake_case（中の型名は PascalCase） | `person.h` / `person.cpp` |

## クラス内のメンバ順序

アクセスレベル順に並べ、**`public:` を先・`private:`（データメンバ）を後**に置く
（C++主流のスタイル。Google C++ Style Guide 等が推奨）。

```cpp
class BankAccount {
public:                       // ① 使い方（外部インターフェース）を先に
  explicit BankAccount(int initialBalance = 0);
  void deposit(int amount);
  int balance() const { return balance_; }

private:                      // ② 実装の詳細（データメンバ）は後に
  int balance_;
};
```

理由：ヘッダはクラスの「使い方」を伝える顔であり、読み手の関心が高い public を先に
置く。データメンバは内部の詳細なので末尾に置く。

## エディタ設定（.vscode/settings.json）

| 設定 | 値 | 意味 |
|------|----|------|
| `editor.formatOnSave` | `true` | 保存時に自動整形 |
| `C_Cpp.formatting` | `clangFormat` | 整形に clang-format を使う |
| `C_Cpp.default.cppStandard` | `c++20` | 補完・解析の言語規格 |
| `editor.rulers` | `[100]` | 100桁目にガイド線 |
| `files.eol` | `\n` | 改行コードを LF に統一 |
