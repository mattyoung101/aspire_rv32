# Original source: https://github.com/five-embeddev/riscv-scratchpad/blob/master/cmake/cmake/riscv.cmake
# usage
# cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/rv32imac.cmake ../
#
# Modified 2023 by Matt to work with Clang.
#
# Require the user to specify the location of the LLVM/GNU toolchain via the RISCV_LLVM_HOME=... env var

message(STATUS "LLVM home: $ENV{RISCV_LLVM_HOME}")

# Look for RISC-V GNU toolchain clang
# https://github.com/riscv/riscv-gnu-toolchain
find_program(RISCV_LLVM_COMPILER "$ENV{RISCV_LLVM_HOME}/bin/riscv32-unknown-elf-clang" REQUIRED)

message("RISC-V LLVM found: ${RISCV_LLVM_COMPILER}")

get_filename_component(RISCV_TOOLCHAIN_BIN_PATH ${RISCV_LLVM_COMPILER} DIRECTORY)
get_filename_component(RISCV_TOOLCHAIN_BIN_CLANG ${RISCV_LLVM_COMPILER} NAME_WE)
get_filename_component(RISCV_TOOLCHAIN_BIN_EXT ${RISCV_LLVM_COMPILER} EXT)

message("RISC-V LLVM toolchain path: ${RISCV_TOOLCHAIN_BIN_PATH}")

STRING(REGEX REPLACE "\-clang" "-" CROSS_COMPILE ${RISCV_TOOLCHAIN_BIN_CLANG})
message("RISC-V cross compile: ${CROSS_COMPILE}")

# The Generic system name is used for embedded targets (targets without OS) in
# CMake
set(CMAKE_SYSTEM_NAME       Generic)
set(CMAKE_SYSTEM_PROCESSOR  rv32ic_zmmul_zicsr)
set(ABI                     "ilp32")
set(CMAKE_EXECUTABLE_SUFFIX ".elf")

# specify the cross compiler. We force the compiler so that CMake doesn't
# attempt to build a simple test program as this will fail without us using
# the -nostartfiles option on the command line
#CMAKE_FORCE_C_COMPILER( "${RISCV_TOOLCHAIN_BIN_PATH}/${CROSS_COMPILE}gcc${RISCV_TOOLCHAIN_BIN_EXT}" GNU )
#CMAKE_FORCE_CXX_COMPILER( "${RISCV_TOOLCHAIN_BIN_PATH}/${CROSS_COMPILE}g++${RISCV_TOOLCHAIN_BIN_EXT}" GNU )
set(CMAKE_ASM_COMPILER "${RISCV_TOOLCHAIN_BIN_PATH}/${CROSS_COMPILE}clang")
set(CMAKE_AR "${RISCV_TOOLCHAIN_BIN_PATH}/llvm-ar")
set(CMAKE_ASM_COMPILER "${RISCV_TOOLCHAIN_BIN_PATH}/${CROSS_COMPILE}clang")
set(CMAKE_C_COMPILER "${RISCV_TOOLCHAIN_BIN_PATH}/${CROSS_COMPILE}clang")
set(CMAKE_CXX_COMPILER "${RISCV_TOOLCHAIN_BIN_PATH}/${CROSS_COMPILE}clang++")

# We must set the OBJCOPY setting into cache so that it's available to the
# whole project. Otherwise, this does not get set into the CACHE and therefore
# the build doesn't know what the OBJCOPY filepath is
set( CMAKE_OBJCOPY      ${RISCV_TOOLCHAIN_BIN_PATH}/llvm-objcopy
     CACHE FILEPATH "The toolchain objcopy command " FORCE )

set( CMAKE_OBJDUMP      ${RISCV_TOOLCHAIN_BIN_PATH}/llvm-objdump
     CACHE FILEPATH "The toolchain objdump command " FORCE )

# Set the common build flags

# Set the CMAKE C flags (which should also be used by the assembler!
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=${CMAKE_SYSTEM_PROCESSOR} -mabi=${ABI}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "")
set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "")
set(CMAKE_EXE_LINKER_FLAGS   "${CMAKE_EXE_LINKER_FLAGS} -march=${CMAKE_SYSTEM_PROCESSOR}")

# Use mold for now, LLD has a bug: https://www.mail-archive.com/llvm-bugs@lists.llvm.org/msg67576.html
# Mold links successfully and is MIT licenced
add_link_options("-fuse-ld=mold")

message(STATUS "End of riscv.cmake")
