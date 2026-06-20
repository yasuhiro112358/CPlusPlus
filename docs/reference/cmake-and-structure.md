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
├── .clang-format               # コード整形ルール（見た目）
├── .clang-tidy                 # 静的解析ルール（バグ・設計、Core Guidelines ベース）
├── .gitignore                  # build/ と .DS_Store を除外
├── scripts/
│   └── tidy.sh                 # clang-tidy ラッパー（macOS の SDK 補完つき）
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
| `clang-format` | コード整形（見た目） | 未インストール（`brew install clang-format`） |
| `clang-tidy` | 静的解析（バグ・設計） | `brew install llvm` で導入（`/opt/homebrew/opt/llvm/bin/`） |

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
| ファイル | snake_case。中身を表す名前 | クラス→型名 `person.h`、自由関数→分野名 `arithmetic.h` |

ファイル名は「中身が何か」を表す。クラスを入れるファイルは型名（実体の名詞、例
`bank_account.h` = `BankAccount`）、自由関数をまとめるファイルは分野名（カテゴリの
名詞、例 `arithmetic.h` = 算術演算）にする。クラスか関数かをファイル名で区別する
マーカー（接頭辞など）は付けない（標準ライブラリ `<vector>` / `<algorithm>` と同じ流儀）。
雑多な補助は `util` ではなく対象を限定（`string_utils` 等）するか分野名を優先する。

## namespace 設計方針

このリポジトリは「ここで実験し、面白くなったプロジェクトは別リポジトリへ引っ越す」
運用を想定している。そのため各プロジェクトを**独立した小さなライブラリの卵**として
設計し、namespace もフラット（傘なし）にする。

| 項目 | 方針 | 理由 |
|------|------|------|
| ルート傘 namespace | **付けない**（フラット） | 引っ越し時に `xxx::` を剥がす手間を無くす |
| namespace 名 | プロジェクト名と同じ（snake_case） | フォルダと一致。引っ越してもそのまま通用 |
| ファイル内専用ヘルパー | 無名 namespace | 外へ出さない関数（例: `buildMessage`） |
| ヘッダで公開する内部詳細 | `<project>::detail` | 複数ファイルで共有する内部用シンボルがある場合のみ |
| プロジェクト内の階層化 | 大きくなったら `<project>::<domain>` | 例: `try_catch::bank`。最初は不要、肥大化したら導入 |

```cpp
// 各プロジェクトはフラットな namespace を持つ
namespace try_catch {
  class BankAccount { ... };
}

// 使う側
try_catch::BankAccount account(100);
```

階層化する場合も、プロジェクトの namespace の**内側**だけで完結させる
（`try_catch::bank` のように）。プロジェクトフォルダごと別リポジトリへ移動しても
壊れない構造を保つ。

### 引っ越しやすさを保つ副次ルール

- プロジェクト間で互いに include しない（依存ゼロ＝フォルダごと持ち出せる）
- 各プロジェクトの `CMakeLists.txt` を自己完結させる
  （トップの `add_subdirectory` を1行消すだけで切り離せる）

### 定義は namespace で囲む（using namespace を使わない）

`.cpp` での定義は、`using namespace X;` ＋ 修飾名ではなく、**全体を
`namespace X { ... }` で囲む**スタイルに統一する（Google C++ Style Guide と同じ流儀）。
自由関数もクラスメンバも同じ形で書け、`using namespace` を全廃できる。

```cpp
// bank_account.cpp
namespace try_catch {
  BankAccount::BankAccount(int initialBalance) : balance_(initialBalance) {}
  void BankAccount::deposit(int amount) { ... }   // クラスメンバ
}

// arithmetic.cpp
namespace calculator {
  int add(int a, int b) { return a + b; }          // 自由関数
}
```

- **ヘッダ（宣言）も実装（定義）も囲む** → 同じ名前空間に宣言と定義が入り対応する。
- **ヘッダでは `using namespace` を絶対に書かない**（include した全ファイルに波及する）。
- 例外：`using namespace` を局所的に使ってよいのは**関数スコープ内**のみ
  （リテラル接尾辞 `using namespace std::chrono_literals;` など）。
  特定の名前1つを局所的に使うなら `using namespace` ではなく
  using 宣言（`using std::swap;`）を関数内で使う。

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

## 単一引数コンストラクタは explicit

引数1つで呼べるコンストラクタには、原則 **`explicit`** を付ける（C++ Core
Guidelines C.46 / Google C++ Style Guide が推奨）。暗黙の型変換を防ぎ、意図を明確にする。

```cpp
class BankAccount {
public:
  explicit BankAccount(int initialBalance = 0);  // ← explicit
};

BankAccount a = 100;   // ❌ explicit があるとエラー（暗黙変換を禁止）
BankAccount a(100);    // ✅ 明示的に書けば OK
```

`explicit` が無いと `int` が黙って `BankAccount` に化ける（`BankAccount a = 100;` が
通ってしまう）。方針「意味が変わる箇所は明示的に」に沿って付ける。

例外：暗黙変換を**意図的に**使いたい場合のみ外す（標準ライブラリの
`std::string s = "hello";` のように、変換が便利な型）。

## main は薄く保つ

`main`（や入口の処理）は**エントリポイントに徹し、ロジックは別ファイルに置く**。
main にはほとんど書かない。

- **ロジックは別ファイルの関数/クラスへ**：テストしやすく、再利用でき、main は流れだけを示す。
- **公開は最小に**：複数の補助関数があるなら、公開窓口を1つ（例 `runAllDemos()`）に絞り、
  個々の補助関数は実装ファイル内の**無名 namespace に隠す**。
- **クラスにするかは状態で決める**：状態が無いなら自由関数を別ファイルにまとめる
  （クラスでラップしない）。

```cpp
// main.cpp — 安全網と1行の呼び出しだけ
#include "exception_demos.h"
int main() {
  try {
    try_catch::runAllDemos();   // 実体は別ファイル
    return 0;
  } catch (const std::exception& e) { ...; return 1; }
  catch (...) { ...; return 1; }
}
```

```cpp
// exception_demos.cpp — 個々の補助は無名 namespace に隠し、窓口だけ公開
namespace try_catch {
  namespace {
    void demoStandardException() { ... }   // file-local（隠す）
    // ...
  }
  void runAllDemos() { ... }               // 公開窓口
}
```

## main は最外 try/catch を持つ

すべての `main` は、本体を `try { ... } catch` で囲む。`throw` を書いていなくても
`std::bad_alloc`（メモリ不足）などの例外はどこでも起こりうるため、main から例外を
漏らして `std::terminate` に至るのを防ぐ安全網を一律で置く。

戻り値は**各分岐の中で返す**（成功は try 内で `return 0`、失敗は catch 内で `return 1`）。
末尾に浮いた `return 0` を置かない。各経路が自分の結末を返すので読みやすい。

```cpp
int main() {
  try {
    // ... 本体 ...
    return 0;
  } catch (const std::exception& e) {
    std::cerr << "予期しない例外が main に到達: " << e.what() << '\n';
    return 1;
  } catch (...) {
    std::cerr << "予期しない非標準例外が main に到達" << '\n';
    return 1;
  }
}
```

ただしこれは C++ 例外に対する安全網であり、未定義動作（範囲外アクセス等）や
OSシグナル（セグフォルト、整数のゼロ除算）は捕まえられない。それらは正しいコードと
静的解析（clang-tidy）で防ぐ。

## 静的解析（.clang-tidy）

バグや非モダンな書き方を指摘する静的解析ツール clang-tidy の設定。
C++ Core Guidelines を中心に、有効化するチェック群を [.clang-tidy](../../.clang-tidy) で管理する。

| チェック群 | 内容 |
|-----------|------|
| `modernize-*` | 古い書き方を現代的な書き方へ（`NULL`→`nullptr`、`typedef`→`using` 等） |
| `cppcoreguidelines-*` | C++ Core Guidelines 違反の指摘 |
| `bugprone-*` | バグになりやすいパターンの検出 |
| `performance-*` | 性能上の問題の指摘 |
| `readability-*` | 可読性（命名規則の自動チェックを含む） |

`readability-identifier-naming` を使い、命名規則（型は PascalCase、メンバ変数は末尾 `_` 等）を
自動チェックしている。実行方法は [how-to/run-static-analysis.md](../how-to/run-static-analysis.md)。

clang-format（見た目の整形）と clang-tidy（バグ・設計の指摘）は**役割が別**。両方併用する。

整形は LLVM プリセットがベース。最大行長は `ColumnLimit: 80`（最も標準的な値）。
日本語コメントが不自然に折られるのを避けるため `ReflowComments: false`（コメントの
折り返しは整形ツールに任せず、長いコメントは著者が改行位置を決める）。

## エディタ設定（.vscode/settings.json）

| 設定 | 値 | 意味 |
|------|----|------|
| `editor.formatOnSave` | `true` | 保存時に自動整形 |
| `C_Cpp.formatting` | `clangFormat` | 整形に clang-format を使う |
| `C_Cpp.default.cppStandard` | `c++20` | 補完・解析の言語規格 |
| `editor.rulers` | `[80]` | 80桁目にガイド線（`.clang-format` の `ColumnLimit` と一致） |
| `files.eol` | `\n` | 改行コードを LF に統一 |
