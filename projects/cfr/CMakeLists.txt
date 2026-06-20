cmake_minimum_required(VERSION 3.20)
project(cfr LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# C++ Core Guidelines を実践するための GSL（gsl::at で境界安全な添字アクセス）。
include(FetchContent)
FetchContent_Declare(
    Microsoft.GSL
    GIT_REPOSITORY https://github.com/microsoft/GSL.git
    GIT_TAG v4.2.2
)
set(GSL_TEST OFF CACHE BOOL "" FORCE) # GSL 自体のテストはビルドしない
FetchContent_MakeAvailable(Microsoft.GSL)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/kuhn_demo.cpp
    src/games/kuhn_game.cpp
    src/games/kuhn_state.cpp
)
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE Microsoft.GSL::GSL)
