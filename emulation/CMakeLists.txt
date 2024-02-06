cmake_minimum_required(VERSION 3.20)
project(emulation C CXX ASM)
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
add_executable(differential_fuzzing differential_fuzzing.cpp baseline_emulator.cpp)
target_include_directories(differential_fuzzing  PRIVATE include)
target_compile_options(differential_fuzzing PRIVATE "-Wall" "-Wextra" "-ggdb")

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
target_link_libraries(differential_fuzzing spdlog::spdlog)

# link libriscv
target_link_libraries(differential_fuzzing riscv)

# Force LLD
add_link_options("-fuse-ld=lld")
set(CMAKE_EXE_LINKER_FLAGS_INIT "-fuse-ld=lld")
set(CMAKE_MODULE_LINKER_FLAGS_INIT "-fuse-ld=lld")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "-fuse-ld=lld")
target_link_options(differential_fuzzing PRIVATE "-fuse-ld=lld")
