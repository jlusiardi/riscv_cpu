`ifndef ARCH_DEFINES
`define ARCH_DEFINES
`include "arch_defines.v"
`endif // ARCH_DEFINES

module compare_unit(
        input [31:0] rs1_value,
        input [31:0] rs2_value,
        input [2:0] cmp_op,
        output jump_condition
    );

    always @(*) begin
        case (cmp_op)
            `RISCV_BEQ: jump_condition = (rs1_value == rs2_value);
            `RISCV_BNE: jump_condition = (rs1_value != rs2_value);
            `RISCV_BLT: jump_condition = ($signed(rs1_value) < $signed(rs2_value));
            `RISCV_BLTU: jump_condition = (rs1_value < rs2_value);
            `RISCV_BGE: jump_condition = ($signed(rs1_value) >= $signed(rs2_value));
            `RISCV_BGEU: jump_condition = (rs1_value >= rs2_value);
            default: jump_condition = 1'b0;
        endcase
   end

endmodule
