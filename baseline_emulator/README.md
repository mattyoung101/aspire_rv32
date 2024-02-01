## Baseline emulator
This directory contains the baseline emulator implemented using libriscv: https://github.com/fwsGonzo/libriscv

### Building and running
Requires CMake 3.20+, Clang, Ninja, LLD and spdlog which can be installed using your package manager.
librscv is fetched automatically by CMake.

Build:

```
cmake -B build -G Ninja
cd build
ninja
```

Run:

```
./baseline_emulator <PATH_TO_BINARY>
```
