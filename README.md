# CPlusPlus

C++ の練習用リポジトリ。試したいテーマごとに `projects/` 配下へ独立したプロジェクトを置く。

詳しいドキュメントは [docs/](docs/README.md) に [Diátaxis](https://diataxis.fr/) 形式で整理している。
はじめての人は [docs/tutorials/getting-started.md](docs/tutorials/getting-started.md) から。

## ディレクトリ構成

```text
.
├── CMakeLists.txt          # 各プロジェクトを add_subdirectory で束ねる
└── projects/
    └── calculator/         # テーマごとに独立した CMake プロジェクト
        ├── CMakeLists.txt
        ├── include/
        └── src/
```

新しいテーマを追加するときは `projects/<theme>/` を作り、トップレベルの
`CMakeLists.txt` に `add_subdirectory(projects/<theme>)` を1行足す。

## ビルドと実行

クローンした後、次の3つの方法のいずれかでビルドできる。

### 方法1：ローカル + CMake（標準）

事前に C++ コンパイラと CMake が必要。macOS なら `clang++` は標準で入っていることが多いので、CMake だけ用意すればよい。

```bash
# 必要なツールの準備（未インストールの場合のみ）
brew install cmake

# Configure
cmake -S . -B build
# Build（全プロジェクト）
cmake --build build
# Run（バイナリは各プロジェクトの binary dir に出力される）
./build/projects/calculator/calculator

# 特定のプロジェクトだけビルドする場合
cmake --build build --target calculator
```

### 方法2：ローカル + clang++ で直接コンパイル

CMake を入れずに、コンパイラだけで動かす方法。ファイルが少ないお試し段階向き。

```bash
clang++ -std=c++20 \
  projects/calculator/src/*.cpp \
  -I projects/calculator/include \
  -o calculator
./calculator
```

### 方法3：Dev Container（環境構築が不要）

[Docker](https://www.docker.com/products/docker-desktop/) と VSCode 拡張
[Dev Containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers)
があれば、ツールを自前で用意せずに動かせる。コンテナ内に CMake や Clang が最初から揃っている。

1. VSCode でこのリポジトリを開く
2. `Cmd+Shift+P` →「Dev Containers: Reopen in Container」
3. コンテナ内のターミナルで方法1のコマンドを実行

ベースイメージ: [devcontainers/images cpp](https://github.com/devcontainers/images/tree/main/src/cpp)


## 命名クイックルール（LLVMベース）
型（class/struct/enum）：PascalCase → Person, Vector2
関数/メソッド：camelCase → getName, setAge
変数：camelCase → userName, count
メンバ変数：camelCase+_ → userName_, count_
定数/マクロ：UPPER_CASE → MAX_SIZE
名前空間：snake_case → myapp::models
ファイル：snake_case（中身の型名はPascalCase）→ person.h / person.cpp

## VSCode Settings
```json
{
    "editor.formatOnSave": true,
    "C_Cpp.formatting": "clangFormat",
    "C_Cpp.default.cppStandard": "c++20",
    "editor.rulers": [
        100
    ],
    "files.eol": "\n"
}
```
`"C_Cpp.formatting": "clangFormat"`でclang-formatを使用できるようになる。
clang-formatはC++コードのフォーマットツール。
設定は`.clang-format`ファイルで行う。
