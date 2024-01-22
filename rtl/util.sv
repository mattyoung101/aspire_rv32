// This file contains typedefs and macros for the whole Aspire project.

typedef enum {
    ALU_ADD, // Signed addition
    ALU_SLL, // Shift left logical
    ALU_XOR, // XOR
    ALU_SRL, // Shift right logical
    ALU_SRA, // Shift right arithmetic
    ALU_OR,  // Logical OR
    ALU_AND, // Logical AND
    ALU_SUB, // Signed subtraction
    ALU_MULS_LO, // Signed multiplication [lower 31:0]
    ALU_MULS_HI, // Signed multiplication [upper 63:32]
    ALU_MULU_LO, // Unsigned multiplication [lower 31:0]
    ALU_MULU_HI // Unsigned multiplication [upper 63:32]
} ALUOp_t;

`ifdef VERILATOR
`define PANIC(msg) $error(msg)
`else
`define PANIC(msg)
`endif
