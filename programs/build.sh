#!/bin/sh
mkdir -p build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../riscv.cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug ..
ninja

# # DEBUG BUILD
# mkdir -p build-debug
# cd build-debug
# cmake -DCMAKE_TOOLCHAIN_FILE=../riscv.cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug ..
# ninja
#
# cd ..
#
# # RELEASE BUILD
# mkdir -p build-release
# cd build-release
# cmake -DCMAKE_TOOLCHAIN_FILE=../riscv.cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
# ninja
