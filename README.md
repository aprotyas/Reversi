# Othello
A 2-player implementation of the popular board game Othello (more generally known as Reversi) in C++.

## Dependencies

- Boost: 1.74.0+
- fmtlib/fmt: 9.1.0+
- CMake: 3.22+
- SDL 2.0
- OpenGL 3.x+
- clang-format 11+

## Build/run/test instructions

I've used GCC 11 for local development, so YMMV for building with other compilers.

```bash
# Configure CMake
cmake -G Ninja -S . -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
# Build
cmake --build build --target othello
# Test
ctest --test-dir build/test
# Run
./build/othello
```

## Commit instructions

`scripts/apply-format` is a formatting script that formats code in this repository using the `clang-format` utility.
Please use this script either on its own or as a pre-commit hook (install using `./scripts/git-pre-commit-format install`) to fix formatting issues before committing code.
Future iterations of CI will fail in a formatting check step otherwise.
