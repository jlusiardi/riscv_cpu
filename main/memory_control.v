`ifndef ARCH_DEFINES
`define ARCH_DEFINES
`include "arch_defines.v"
`endif // ARCH_DEFINES

module memory_control(
      // Inputs
      input [2:0] stage,
      input [6:0] opcode,

      input [31:0] pc_value,
      input [31:0] offset,
      input [31:0] rs1_value,
      input [31:0] memory_write_data,

      // Outputs to send to main memory
      output [31:0] read_address,
      output [31:0] write_address,
      output [31:0] write_data,
      output        write_enable
   );

   wire current_instr_is_store;
   assign current_instr_is_store = opcode == `RISCV_STORE;

   assign write_enable = (stage == `STAGE_MEMORY) && current_instr_is_store;

   reg [31:0] read_address_i;

   assign write_data = memory_write_data;
   assign write_address = rs1_value + offset;

   always @(*) begin
      if (stage == `STAGE_FETCH) begin
         read_address_i = pc_value;
      end else begin
          read_address_i = rs1_value + offset;
      end
   end

   assign read_address = read_address_i;

endmodule
