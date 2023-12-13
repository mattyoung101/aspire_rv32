#!/bin/sh
mkdir -p build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../riscv.cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
ninja
