`ifndef ARCH_DEFINES
`define ARCH_DEFINES
`include "arch_defines.v"
`endif // ARCH_DEFINES

module alu_control(
        input [6:0] opcode,

        input [31:0] rs1,
        input [31:0] rs2,
        input [31:0] pc_value,
        input [31:0] immediate,

        // Outputs sent to ALU
        output [31:0] alu_in1,
        output [31:0] alu_in2
    );

    reg [31:0] alu_in1_i;
    reg [31:0] alu_in2_i;

    always @(*) begin
        if (opcode == `RISCV_AUIPC) begin
            alu_in1_i = pc_value;
            alu_in2_i = immediate;
        end else if (opcode == `RISCV_OP) begin
            alu_in1_i = rs1;
            alu_in2_i = rs2;
        end else if (opcode == `RISCV_OP_IMM) begin
            alu_in1_i = rs1;
            alu_in2_i = immediate;
        end else begin
            alu_in1_i = 32'b0;
            alu_in2_i = 32'b0;
        end
    end

    assign alu_in1 = alu_in1_i;
    assign alu_in2 = alu_in2_i;

endmodule
