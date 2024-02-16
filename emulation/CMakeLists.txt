cmake_minimum_required(VERSION 3.20)
project(emulation C CXX ASM)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11) # C11
set(CMAKE_CXX_STANDARD 20) # C++23

# Baseline emulator executable
add_executable(emulator 
    src/emulator.cpp 
    src/baseline_emulator.cpp
    src/rv.c
    src/riscv-disas.c
    src/mmio.cpp
    src/simstop.cpp
    src/uart.cpp
    src/wdog.cpp
)
target_include_directories(emulator PRIVATE 
    include 
)
target_compile_options(emulator PRIVATE "-Wall" "-Wextra" "-ggdb")
target_compile_definitions(emulator PRIVATE "-DRISCV_EM_DEBUG")

if ("${CMAKE_BUILD_TYPE}" STREQUAL Release)
    message(STATUS "Release build")
    target_compile_options(emulator PRIVATE "-O3" "-march=native" "-mtune=native")
    target_link_options(emulator PRIVATE "-flto=thin")
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL Debug)
    message(STATUS "Debug build, adding sanitizers")
    target_compile_options(emulator PRIVATE "-fsanitize=address" "-fsanitize=undefined"
    "-fno-omit-frame-pointer")
    target_link_options(emulator PRIVATE "-fsanitize=address" "-fsanitize=undefined")
endif()

# spdlog
find_package(spdlog REQUIRED)
target_link_libraries(emulator spdlog::spdlog)

# Force LLD
add_link_options("-fuse-ld=lld")
set(CMAKE_EXE_LINKER_FLAGS_INIT "-fuse-ld=lld")
set(CMAKE_MODULE_LINKER_FLAGS_INIT "-fuse-ld=lld")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "-fuse-ld=lld")
target_link_options(emulator PRIVATE "-fuse-ld=lld")
