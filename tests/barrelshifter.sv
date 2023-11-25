module barrelshifter(
    input logic clk,
    input logic[31:0] a,
    input logic[31:0] b,
    input logic opcode,
    output logic[31:0] out
);
    always_ff @(posedge clk) begin
        if (opcode)
            out <= a << b;
        else
            out <= a >> b;
    end
endmodule
