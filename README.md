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