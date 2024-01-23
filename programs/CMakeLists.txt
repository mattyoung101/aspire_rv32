# References:
# - https://github.com/five-embeddev/riscv-scratchpad/blob/master/cmake/src/CMakeLists.txt
# - https://github.com/jobroe/cmake-arm-embedded/blob/master/toolchain-arm-none-eabi.cmake
# - https://github.com/UQRacing/cmu22/blob/master/CMakeLists.txt
# - https://github.com/riscv-software-src/riscv-tests/blob/master/benchmarks/Makefile
cmake_minimum_required(VERSION 3.20)
project(riscv-programs C CXX ASM)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(coremark 
    # Base CoreMark
    coremark/core_list_join.c 
    coremark/core_main.c 
    coremark/core_matrix.c
    coremark/core_state.c 
    coremark/core_util.c 

    # RV32 port
    coremark_rv32/core_portme.c 
    coremark_rv32/ee_printf.c

    # Aspire HAL
    lib/aspire.c
    lib/syscalls.c
    lib/crt.S
)

add_executable(hello_world 
    hello_world.c 
    
    # Aspire HAL
    lib/aspire.c
    lib/syscalls.c
    lib/crt.S
)

# FIXME we need an RV32 bare metal linker script

target_include_directories(coremark PRIVATE coremark)
target_include_directories(coremark PRIVATE coremark_rv32)
target_include_directories(coremark PRIVATE lib)
target_include_directories(hello_world PRIVATE lib)

# Shared compile options
target_compile_options(coremark PRIVATE "-Wall" "-Wextra" "-ffunction-sections" "-fdata-sections" "-static" "-fno-common")
target_compile_options(hello_world PRIVATE "-Wall" "-Wextra" "-ffunction-sections" "-fdata-sections" "-static"
"-fno-common")

# Specify CoreMark iteration count
target_compile_definitions(coremark PRIVATE "-DITERATIONS=1000")

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Release build, using min size")

    # Use -Oz, no symbols
    target_compile_options(coremark PRIVATE "-Oz")
    target_compile_options(hello_world PRIVATE "-Oz")

    # Strip symbols
    set_target_properties(coremark PROPERTIES LINK_FLAGS_RELEASE -s)
    set_target_properties(coremark PROPERTIES LINK_FLAGS -s)
    set_target_properties(hello_world PROPERTIES LINK_FLAGS_RELEASE -s)
    set_target_properties(hello_world PROPERTIES LINK_FLAGS -s)

    # Define compile flags for CoreMark
    target_compile_definitions(coremark PRIVATE "-DCOMPILER_FLAGS=\"${CMAKE_C_FLAGS} -Oz\"")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Debug build")

    # Use -Og and write symbols
    target_compile_options(coremark PRIVATE "-Og" "-ggdb")
    target_compile_options(hello_world PRIVATE "-Og" "-ggdb")

    # Define compile flags for CoreMark
    target_compile_definitions(coremark PRIVATE "-DCOMPILER_FLAGS=\"${CMAKE_C_FLAGS} -Og\"")
endif()

# Link options
# TODO linker script
# TODO do we want -nostdlib -nostartfiles?

# target_link_options(coremark PRIVATE -Wl,--gc-sections,--print-memory-usage,-Map=coremark.map)
# target_link_options(coremark PRIVATE -Wl,--specs=nano.specs)
# add_link_options(--specs=nano.specs)
# add_link_options(--specs=nosys.specs)

# set(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections,--print-memory-usage,-Map=coremark.map -Wl,--specs=nano.specs")

#target_link_options(coremark PRIVATE "-lnosys")

target_link_options(coremark PRIVATE -nostdlib -nostartfiles -lgcc -T ${CMAKE_SOURCE_DIR}/lib/test.ld)
target_link_libraries(coremark PRIVATE gcc)

target_link_options(hello_world PRIVATE -nostdlib -nostartfiles -lgcc -T ${CMAKE_SOURCE_DIR}/lib/test.ld)
target_link_libraries(hello_world PRIVATE gcc)

# # Post processing command to create a hex file 
# add_custom_command(TARGET ${TARGET}.elf POST_BUILD
#         COMMAND ${CMAKE_OBJCOPY} -O ihex  ${TARGET}.elf  ${TARGET}.hex
#         COMMENT "Invoking: Hexdump")
