# はじめてのビルドと実行

このチュートリアルでは、リポジトリをクローンした状態から、最初のプログラム
（`calculator`）をビルドして動かすところまでを体験する。この通りに進めれば必ず動く。

所要時間：約5分（CMake のインストール時間を除く）

## 前提

- macOS を使っている
- ターミナルが開ける
- [Homebrew](https://brew.sh/) が入っている

## ステップ1：コンパイラがあるか確認する

macOS には C++ コンパイラ（Clang）が標準で入っていることが多い。確認する。

```bash
clang++ --version
```

`Apple clang version ...` と表示されれば準備OK。表示されない場合は Xcode Command
Line Tools を入れる：`xcode-select --install`

## ステップ2：CMake を入れる

ビルドを組み立てる CMake を入れる。

```bash
brew install cmake
```

入ったか確認する：

```bash
cmake --version
```

## ステップ3：ビルドを設定する（Configure）

リポジトリのルートで次を実行する。`build/` というフォルダにビルドの段取りが作られる。

```bash
cmake -S . -B build
```

最後に `Build files have been written to: .../build` と出れば成功。

## ステップ4：ビルドする（Build）

```bash
cmake --build build
```

`[100%] Built target calculator` と出れば、実行ファイルが完成している。

## ステップ5：実行する

```bash
./build/projects/calculator/calculator
```

次のように表示されれば成功：

```text
Hello from calculator!
2 + 3 = 5
5 - 3 = 2
```

おめでとう。これが C++ プロジェクトをビルドして動かす一連の流れ。

## 次に何を見るか

- コードを書き換えて再ビルドしたい → ステップ4の `cmake --build build` をもう一度実行するだけ
- 新しいテーマを追加したい → [how-to/add-new-project.md](../how-to/add-new-project.md)
- CMake が裏で何をしているのか知りたい → [explanation/build-system-and-environment.md](../explanation/build-system-and-environment.md)
