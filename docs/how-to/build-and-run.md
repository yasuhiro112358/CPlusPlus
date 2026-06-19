# ビルドして実行する

クローンした後、3つの方法のいずれかでビルドできる。状況に応じて選ぶ。

| 方法 | 前提 | 向いている場面 |
|------|------|--------------|
| 1. ローカル + CMake | clang++ + cmake | 標準的なビルド |
| 2. ローカル + 直接コンパイル | clang++ のみ | お試し・最小構成 |
| 3. Dev Container | Docker + VSCode拡張 | 環境構築なしで動かす・他人と共有 |

## 方法1：ローカル + CMake（標準）

```bash
# 未インストールの場合のみ
brew install cmake

# Configure（段取りを build/ に生成）
cmake -S . -B build
# Build（全プロジェクト）
cmake --build build
# Run
./build/projects/calculator/calculator
```

特定のプロジェクトだけビルドするには `--target` を指定する：

```bash
cmake --build build --target calculator
```

コードを修正した後は `cmake --build build` を再実行するだけでよい（Configure は不要）。

## 方法2：ローカル + clang++ で直接コンパイル

CMake を使わず、コンパイラだけで動かす。ファイルが少ない段階向き。

```bash
clang++ -std=c++20 \
  projects/calculator/src/*.cpp \
  -I projects/calculator/include \
  -o calculator
./calculator
```

## 方法3：Dev Container

[Docker Desktop](https://www.docker.com/products/docker-desktop/) と VSCode 拡張
[Dev Containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers)
が必要。コンテナ内に CMake や Clang が最初から揃っている。

1. VSCode でこのリポジトリを開く
2. `Cmd+Shift+P` →「Dev Containers: Reopen in Container」
3. コンテナ内のターミナルで方法1のコマンドを実行する

## ビルドをリセットしたい

`build/` は生成物しか入っていないので、丸ごと削除して作り直せる。
`CMakeLists.txt` を変更したのに反映されないときの確実なリセット方法でもある。

```bash
rm -rf build
cmake -S . -B build
```

## 関連

- ビルドの仕組みを理解したい → [explanation/build-system-and-environment.md](../explanation/build-system-and-environment.md)
- `build/` の中身が何か知りたい → [reference/cmake-and-structure.md](../reference/cmake-and-structure.md)
