cmake_minimum_required(VERSION 3.20)
project(try_catch LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/exception_demos.cpp
    src/bank_account.cpp
    src/insufficient_funds_error.cpp
)
target_include_directories(${PROJECT_NAME} PRIVATE include)
