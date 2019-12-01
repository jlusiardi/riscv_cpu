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

    reg jump_condition_reg;

    always @(*) begin
        case (cmp_op)
            `RISCV_BEQ: jump_condition_reg = (rs1_value == rs2_value);
            `RISCV_BNE: jump_condition_reg = (rs1_value != rs2_value);
            `RISCV_BLT: jump_condition_reg = ($signed(rs1_value) < $signed(rs2_value));
            `RISCV_BLTU: jump_condition_reg = (rs1_value < rs2_value);
            `RISCV_BGE: jump_condition_reg = ($signed(rs1_value) >= $signed(rs2_value));
            `RISCV_BGEU: jump_condition_reg = (rs1_value >= rs2_value);
            default: jump_condition_reg = 1'b0;
        endcase
   end

   assign jump_condition = jump_condition_reg;

endmodule
