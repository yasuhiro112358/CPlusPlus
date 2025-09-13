# CPlusPlus


### dev Container for C++ development.
https://github.com/devcontainers/images/tree/main/src/cpp


cmake --version


### How to build and run without CMake Presets
```bash
# Configure
cmake -S . -B build 
# Build
cmake --build build
# Run
./build/cpp_lab
```

###  Using CMake Presets
```bash
# Configure
cmake --preset default
# Build
cmake --build --preset build
# Run
./build/cpp_lab
```


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
