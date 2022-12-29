# Othello
A 2-player implementation of the popular board game Othello (more generally known as Reversi) in C++.

## Build/run instructions

I've used GCC 11 for local development, so YMMV for building with other compilers.

```bash
# Configure CMake
cmake -G Ninja -S . -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
# Build
cmake --build build --target othello
# Run
./build/othello
```
