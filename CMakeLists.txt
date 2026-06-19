cmake_minimum_required(VERSION 3.20)
project(CPlusPlus LANGUAGES CXX)

# clang-tidy / clangd が解析に使う compile_commands.json を build/ に出力する
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(projects/calculator)
add_subdirectory(projects/try_catch)
add_subdirectory(projects/cfr)
