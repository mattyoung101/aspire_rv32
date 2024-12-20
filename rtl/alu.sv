`default_nettype none
`include "util.sv"

// This module implements Aspire's ALU. This ALU is implemented purely combinatorially. There's also W, the
// word size (XLEN), which is always 32 for this processor but can be reconfigured "just in case".
module alu # (
    parameter W = 32 // Word width (32-bit on RV32)
) (
    input logic[W-1:0] i_a, // Input A
    input logic[W-1:0] i_b,  // Input B
    input ALUOp_t i_op, // ALU op
    output logic[W-1:0] o_out // ALU output
);
    // This is the full 2*XLEN multiplication result. Currently we compute _BOTH_ signed and unsigned, because
    // I'm hoping that Yosys can optimise the area of this. However, in future, it would be better if we
    // multiplex the input operands (i.e., choosing if i_a/i_b is signed based on the ALU op)
    logic[2*W-1:0] muls;
    logic[2*W-1:0] mulu;
    assign muls = $signed(i_a) * $signed(i_b);
    assign mulu = $unsigned(i_a) * $unsigned(i_b);

    always_comb begin: alu_comb
        unique case (i_op)
            // Add sub
            ALU_ADD: o_out = $signed(i_a) + $signed(i_b); // TODO consider signed' cast?
            ALU_SUB: o_out = $signed(i_a) - $signed(i_b);

            // Shifts (TODO do these need to be signed)
            ALU_SLL: o_out = i_a << i_b;
            ALU_SRL: o_out = i_a >>> i_b;
            ALU_SRA: o_out = i_a >> i_b;

            // Bitwise
            ALU_XOR: o_out = i_a ^ i_b;
            ALU_AND: o_out = i_a & i_b;
            ALU_OR: o_out = i_a | i_b;
            
            // Multiplication
            ALU_MULS_LO: o_out = muls[W-1:0];
            ALU_MULS_HI: o_out = muls[2*W-1:32];

            ALU_MULU_LO: o_out = mulu[W-1:0];
            ALU_MULU_HI: o_out = mulu[2*W-1:32];

            default: begin
                o_out = 0;
                `PANIC("Unhandled ALU operand!");
            end
        endcase
    end
endmodule
