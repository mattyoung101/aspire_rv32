module mult(
    input logic clk,
    input logic[31:0] a,
    input logic[31:0] b,
    output logic[31:0] out
);
    always_ff @(posedge clk) begin
        out <= a * b;
    end
endmodule
