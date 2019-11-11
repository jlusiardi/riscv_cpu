`ifndef ARCH_DEFINES
`define ARCH_DEFINES
`include "arch_defines.v"
`endif // ARCH_DEFINES

module pc_control(
      input [6:0] opcode,
      input [2:0] stage,
      input jump_condition,
      input [31:0] pc_input,
      input [31:0] offset,
      input [31:0] rs1,

      output [31:0] pc_output,
      output pc_en
    );

   reg [31:0] pc_intern;

   always @(*) begin
      if (opcode == `RISCV_BRANCH) begin
         if (jump_condition == 1'b1) begin
            pc_intern = pc_input + offset;
         end
      end else if (opcode == `RISCV_JALR) begin
         pc_intern = rs1 + offset;
      end else if (opcode == `RISCV_JAL) begin
         pc_intern = pc_input + offset;
      end else begin
         pc_intern = pc_input + 4;
      end
   end

   assign pc_output = pc_intern;
   assign pc_en = (stage == `STAGE_REGISTER_UPDATE);
endmodule
