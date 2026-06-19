# 新しいテーマ（プロジェクト）を追加する

このリポジトリは、試したいテーマごとに `projects/<theme>/` へ独立した CMake
プロジェクトを置く構成になっている。新しいテーマを追加する手順を示す。

ここでは例として `strings` というテーマを追加する。

## ステップ1：ディレクトリを作る

```bash
mkdir -p projects/strings/src projects/strings/include
```

## ステップ2：プロジェクトの CMakeLists.txt を作る

`projects/strings/CMakeLists.txt` を次の内容で作成する。`calculator` の
[CMakeLists.txt](../../projects/calculator/CMakeLists.txt) をベースにすると速い。

```cmake
cmake_minimum_required(VERSION 3.20)
project(strings LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME}
    src/main.cpp
)
target_include_directories(${PROJECT_NAME} PRIVATE include)
```

ソースを増やしたら `add_executable` に `.cpp` を追記していく。

## ステップ3：ソースを書く

`projects/strings/src/main.cpp`：

```cpp
#include <iostream>

int main() {
  std::cout << "Hello from strings!" << std::endl;
  return 0;
}
```

## ステップ4：トップレベルの CMakeLists.txt に登録する

リポジトリルートの [CMakeLists.txt](../../CMakeLists.txt) に1行追加する。

```cmake
add_subdirectory(projects/strings)
```

## ステップ5：ビルドして実行する

```bash
cmake -S . -B build          # 新プロジェクトを認識させるため再Configure
cmake --build build
./build/projects/strings/strings
```

新しいプロジェクトを追加した直後は再 Configure（`cmake -S . -B build`）が必要な点に注意。
既存プロジェクトのコード修正だけなら `cmake --build build` で足りる。

## 関連

- なぜこの独立構成にしているのか → [explanation/build-system-and-environment.md](../explanation/build-system-and-environment.md)
- 命名規則 → [reference/cmake-and-structure.md](../reference/cmake-and-structure.md)
