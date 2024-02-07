cmake_minimum_required(VERSION 3.20)
project(emulation C CXX ASM)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

# Find Sail directory
message(STATUS "Locating Sail")
execute_process(COMMAND "opam" "var" "sail:share" 
    OUTPUT_VARIABLE SAIL_SHARE
    ERROR_VARIABLE SAIL_SHARE_ERROR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "Sail share: ${SAIL_SHARE}")

if (SAIL_SHARE STREQUAL "")
    message(FATAL_ERROR "Sail not found! Error: ${SAIL_SHARE_ERROR}")
endif()

set(SAIL_LIB "${SAIL_SHARE}/lib")
message(STATUS "Sail lib: ${SAIL_LIB}")

# Baseline emulator executable
add_executable(differential_fuzzing 
    differential_fuzzing.cpp 
    baseline_emulator.cpp

    # Sail
    ${SAIL_LIB}/sail_failure.c
    ${SAIL_LIB}/sail.c
    ${SAIL_LIB}/rts.c
    ${SAIL_LIB}/elf.c

    # Sail RISC-V
    sail/riscv_model_RV32.c
    sail/riscv_platform.c
    sail/riscv_platform_impl.c
    sail/riscv_prelude.c
    sail/riscv_softfloat.c
)
target_include_directories(differential_fuzzing PRIVATE include ${SAIL_LIB} sail) 
target_compile_options(differential_fuzzing PRIVATE "-Wall" "-Wextra" "-Wno-unused-parameter"
    "-Wno-sign-compare" "-ggdb")

if ("${CMAKE_BUILD_TYPE}" STREQUAL Release)
    message(STATUS "Release build")
    # TODO optimisations
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL Debug)
    message(STATUS "Debug build, adding sanitizers")
    target_compile_options(differential_fuzzing PRIVATE "-fsanitize=address" "-fsanitize=undefined"
    "-fno-omit-frame-pointer")
    target_link_options(differential_fuzzing PRIVATE "-fsanitize=address" "-fsanitize=undefined")
endif()

# spdlog
find_package(spdlog REQUIRED)
target_link_libraries(differential_fuzzing spdlog::spdlog gmp z)

# Force LLD
add_link_options("-fuse-ld=lld")
set(CMAKE_EXE_LINKER_FLAGS_INIT "-fuse-ld=lld")
set(CMAKE_MODULE_LINKER_FLAGS_INIT "-fuse-ld=lld")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "-fuse-ld=lld")
target_link_options(differential_fuzzing PRIVATE "-fuse-ld=lld")
