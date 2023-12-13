# References:
# - https://github.com/five-embeddev/riscv-scratchpad/blob/master/cmake/src/CMakeLists.txt
cmake_minimum_required(VERSION 3.20)
project(riscv-programs C CXX ASM)

set (STACK_SIZE 0xf00)

add_executable(coremark coremark/core_list_join.c coremark/core_main.c coremark/core_matrix.c
coremark/core_state.c coremark/core_util.c core_portme.c)

# FIXME we need an RV32 bare metal linker script
# set(LINKER_SCRIPT "riscv-coremark/riscv64-baremetal/link.ld")
# set_target_properties(coremark PROPERTIES LINK_DEPENDS "${LINKER_SCRIPT}")

target_include_directories(coremark PRIVATE coremark)
target_include_directories(coremark PRIVATE ${CMAKE_SOURCE_DIR})
target_compile_options(coremark PRIVATE "-Wall" "-Wextra" "-Oz")

# Linker control
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fno-exceptions -Xlinker --defsym=__stack_size=${STACK_SIZE}")

target_compile_definitions(coremark PRIVATE "-DITERATIONS=1000")

set_target_properties(coremark PROPERTIES LINK_FLAGS_RELEASE -s)
set_target_properties(coremark PROPERTIES LINK_FLAGS -s)

# Post processing command to create a disassembly file 
# add_custom_command(TARGET coremark POST_BUILD
#         COMMAND ${CMAKE_OBJDUMP} -S  ${TARGET}.elf > ${TARGET}.disasm
#         COMMENT "Invoking: Disassemble")
#
# # Post processing command to create a hex file 
# add_custom_command(TARGET ${TARGET}.elf POST_BUILD
#         COMMAND ${CMAKE_OBJCOPY} -O ihex  ${TARGET}.elf  ${TARGET}.hex
#         COMMENT "Invoking: Hexdump")
