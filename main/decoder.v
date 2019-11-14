`ifndef ARCH_DEFINES
`define ARCH_DEFINES
`include "arch_defines.v"
`endif // ARCH_DEFINES

module decoder(
        /* verilator lint_off UNUSED */
        // Only some bits are used for the various immediate types
        input [31:0]  instruction,
        /* verilator lint_on UNUSED */

        // Outputs
        output [6:0]  opcode,
        output [2:0]  func3,
        output [6:0]  func7,
        output [31:0] imm,
        output instr_valid,
        output [4:0]  reg_source_0,
        output [4:0]  reg_source_1,
        output [4:0]  reg_dest
    );
   
    assign opcode = instruction[6:0];
    assign func3 = instruction[14:12];
    assign func7 = instruction[31:25];

    wire [31:0] i_imm = {{21{instruction[31]}}, instruction[30:20]};
    wire [31:0] s_imm = {{21{instruction[31]}}, instruction[30:25], instruction[11:7]};
    wire [31:0] b_imm = {{20{instruction[31]}}, instruction[7], instruction[30:25], instruction[11:8], 1'b0};
    wire [31:0] u_imm = {instruction[31:12], {12{1'b0}}};
    wire [31:0] j_imm = {{12{instruction[31]}}, instruction[19:12], instruction[20], instruction[30:21], 1'b0};

    always @(*)
    begin
        if (opcode == `RISCV_JALR)  begin
            imm = i_imm;
        end
        else if (opcode == `RISCV_LOAD) begin
            imm = i_imm;
            instr_valid = 1'b1;
        end
        else if (opcode == `RISCV_ALU_OP_REGS) begin
            imm = 32'b0;
            instr_valid = 1'b1;
        end
        else if (opcode == `RISCV_ALU_OP_IMM) begin
            imm = i_imm;
            instr_valid = 1'b1;
        end
        else if (opcode == `RISCV_STORE) begin
            imm = s_imm;
            instr_valid = (func3 == `STORE_BYTE) 
                        | (func3 == `STORE_HALFWORD) 
                        | (func3 ==  `STORE_WORD) ;
        end
        else if (opcode == `RISCV_BRANCH) begin
            imm = b_imm;
            instr_valid = (func3 == `BRANCH_EQ) 
                        | (func3 == `BRANCH_NE) 
                        | (func3 == `BRANCH_LT) 
                        | (func3 == `BRANCH_GE) 
                        | (func3 == `BRANCH_LTU) 
                        | (func3 == `BRANCH_GEU);
        end
        else if (opcode == `RISCV_LUI) begin
            imm = u_imm;
            instr_valid = 1'b1;
        end
        else if (opcode == `RISCV_AUIPC) begin
            imm = u_imm;
            instr_valid = 1'b1;
        end
        else if (opcode == `RISCV_JAL) begin
            imm = j_imm;
            instr_valid = 1'b1;
        end
        // TODO include fence, ecall and ebreak here
        else begin
            imm = 32'b0;
            instr_valid = 1'b0;
        end
    end

    assign reg_source_0 = instruction[19:15];
    assign reg_source_1 = instruction[24:20];
    assign reg_dest = instruction[11:7];
endmodule
