# References:
# - https://github.com/five-embeddev/riscv-scratchpad/blob/master/cmake/src/CMakeLists.txt
# - https://github.com/jobroe/cmake-arm-embedded/blob/master/toolchain-arm-none-eabi.cmake
# - https://github.com/UQRacing/cmu22/blob/master/CMakeLists.txt
# - https://github.com/riscv-software-src/riscv-tests/blob/master/benchmarks/Makefile
# - https://github.com/YosysHQ/picorv32/blob/master/picosoc/Makefile
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

    # LibC stuff
    lib/crt.S
    lib/syscalls.c
)

add_executable(hello_world 
    hello_world.c 
    
    # Aspire HAL
    lib/aspire.c

    # LibC stuff
    lib/crt.S
    lib/syscalls.c
)

target_include_directories(coremark PRIVATE coremark)
target_include_directories(coremark PRIVATE coremark_rv32)
target_include_directories(coremark PRIVATE lib)
target_include_directories(hello_world PRIVATE lib)

# Shared compile options
target_compile_options(coremark PRIVATE "-Wall" "-Wextra" "-ffunction-sections" "-fdata-sections" "-static"
    "-fno-common" "-ffreestanding")
target_compile_options(hello_world PRIVATE "-Wall" "-Wextra" "-ffunction-sections" "-fdata-sections" "-static" 
    "-fno-common" "-ffreestanding")

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
    message(STATUS "Debug build, writing symbols")

    # Use -Og and write symbols
    target_compile_options(coremark PRIVATE "-Og" "-ggdb")
    target_compile_options(hello_world PRIVATE "-Og" "-ggdb")

    # Define compile flags for CoreMark
    target_compile_definitions(coremark PRIVATE "-DCOMPILER_FLAGS=\"${CMAKE_C_FLAGS} -Og\"")
endif()

# Link options
target_link_options(coremark PRIVATE -nostdlib -nodefaultlibs -lgcc -T ${CMAKE_SOURCE_DIR}/lib/aspire.ld)
target_link_libraries(coremark PRIVATE gcc)

target_link_options(hello_world PRIVATE -nostdlib -nodefaultlibs -lgcc -T ${CMAKE_SOURCE_DIR}/lib/aspire.ld)
target_link_libraries(hello_world PRIVATE gcc)

# Garbage collect unused sections
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections,--print-gc-sections,--print-memory-usage")

# Post-processing to create a .bin file (for emulators and such)
add_custom_command(TARGET coremark POST_BUILD 
    COMMAND ${CMAKE_OBJCOPY} -O binary ${CMAKE_BINARY_DIR}/coremark ${CMAKE_BINARY_DIR}/coremark.bin
    COMMENT "objcopy bin: coremark"
)

add_custom_command(TARGET hello_world POST_BUILD 
    COMMAND ${CMAKE_OBJCOPY} -O binary ${CMAKE_BINARY_DIR}/hello_world ${CMAKE_BINARY_DIR}/hello_world.bin
    COMMENT "objcopy bin: helloworld"
)

# Post-processing to create a .hex file (for the SPI flash chip)
add_custom_command(TARGET coremark POST_BUILD 
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${CMAKE_BINARY_DIR}/coremark ${CMAKE_BINARY_DIR}/coremark.hex
    COMMENT "objcopy hex: coremark"
)

add_custom_command(TARGET hello_world POST_BUILD 
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${CMAKE_BINARY_DIR}/hello_world ${CMAKE_BINARY_DIR}/hello_world.hex
    COMMENT "objcopy hex: hello_world"
)
