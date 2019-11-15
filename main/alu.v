`ifndef ARCH_DEFINES
`define ARCH_DEFINES
`include "arch_defines.v"
`endif // ARCH_DEFINES

module alu(
        input [31:0] rs1,
        input [31:0] rs2,
        input [6:0] opcode,
        input [2:0] func3,
        input [6:0] func7,

        output [31:0] out
    );

   wire [31:0] out_add;
   assign out_add = rs1 + rs2;

   wire [31:0] out_sub;
   assign out_sub = rs1 - rs2;

   wire [31:0] out_xor;
   assign out_xor = rs1 ^ rs2;

   wire [31:0] out_or;
   assign out_or = rs1 | rs2;

   wire [31:0] out_and;
   assign out_and = rs1 & rs2;

   wire [31:0] out_slt;
   assign out_slt = $signed(rs1) < $signed(rs2) ? {{31{1'b0}}, {1'b1}} : 32'b0;

   wire [31:0] out_sltu;
   assign out_sltu = rs1 < rs2 ? {{31{1'b0}}, {1'b1}} : 32'b0;

   wire [31:0] out_sll;
   assign out_sll = rs1 << (rs2 & {{27{1'b0}}, {5{1'b1}}});

   wire [31:0] out_srl;
   assign out_srl = rs1 >> (rs2 & {{27{1'b0}}, {5{1'b1}}});

   wire [31:0] out_sra;
   assign out_sra = $signed(rs1) >>> (rs2 & {{27{1'b0}}, {5{1'b1}}});

   reg [31:0] out_r;
   always @(*) begin
      
      case (opcode)
        `RISCV_AUIPC: begin
            out_r = rs1 + rs2;
        end
        `RISCV_OP_IMM: begin
          case (func3)
            // ADDI
            `RISCV_ADD: out_r = out_add;
            // SLLI
            `RISCV_SLL: out_r = out_sll;
            // XORI
            `RISCV_XOR: out_r = out_xor;
            // ORI
            `RISCV_OR: out_r = out_or;
            // ANDI
            `RISCV_AND: out_r = out_and;
            `RISCV_SRL: begin
              case(func7)
                // SRLI
                `RISCV_STD_OP: out_r = out_srl;
                // SRAI
                `RISCV_ALT_OP: out_r = out_sra;
                default: out_r = 32'h0;
              endcase
            end
            `RISCV_SLT: out_r = out_slt;
            `RISCV_SLTU: out_r = out_sltu;
          default: out_r = 32'h0;
          endcase
        end
        `RISCV_OP: begin
          case (func3)
            `RISCV_SLL: out_r = out_sll;
            // XOR
            `RISCV_XOR: out_r = out_xor;
            // OR
            `RISCV_OR: out_r = out_or;
            // AND
            `RISCV_AND: out_r = out_and;
            `RISCV_SLT: out_r = out_slt;
            `RISCV_SLTU: out_r = out_sltu;
            `RISCV_ADD: begin
              case(func7)
                `RISCV_STD_OP: out_r = out_add;
                `RISCV_ALT_OP: out_r = out_sub;
                default: out_r = 32'h0;
              endcase
            end
            `RISCV_SRL: begin
              case(func7)
                // SRL
                `RISCV_STD_OP: out_r = out_srl;
                // SRA
                `RISCV_ALT_OP: out_r = out_sra;
                default: out_r = 32'h0;
              endcase
            end
            default: out_r = 32'h0;
          endcase
        end
        
        default: out_r = 32'h0;
      endcase
   end

   assign out = out_r;
   
endmodule
