cmake_minimum_required(VERSION 3.20)
project(baseline_emulator C CXX ASM)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

# Acquire libriscv
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
FetchContent_Declare(libriscv
  GIT_REPOSITORY https://github.com/fwsGonzo/libriscv
  GIT_TAG        v1.1 # or any later version
  GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(libriscv)

# Baseline emulator executable
# TODO optimisations
add_executable(baseline_emulator baseline_emulator.cpp)
target_compile_options(baseline_emulator PRIVATE "-Wall" "-Wextra" "-ggdb")

# spdlog
find_package(spdlog REQUIRED)
target_link_libraries(baseline_emulator spdlog::spdlog)

# link libriscv
target_link_libraries(baseline_emulator riscv)

# Force LLD
add_link_options("-fuse-ld=lld")
set(CMAKE_EXE_LINKER_FLAGS_INIT "-fuse-ld=lld")
set(CMAKE_MODULE_LINKER_FLAGS_INIT "-fuse-ld=lld")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "-fuse-ld=lld")
target_link_options(baseline_emulator PRIVATE "-fuse-ld=lld")
