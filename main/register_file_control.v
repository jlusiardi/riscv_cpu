`ifndef ARCH_DEFINES
`define ARCH_DEFINES
`include "../common/arch_defines.v"
`endif // ARCH_DEFINES

module register_file_control(
        input `STAGE_WIDTH stage,
        input [6:0] opcode,
        input [2:0] func3,
        input [31:0] alu_result,
        input [31:0] immediate,
        input [31:0] memory_result,
        input [31:0] csr_result,
        input [31:0] pc,

        output write_enable,
        output [31:0] output_value
    );

    wire use_alu_result;
    assign use_alu_result = (opcode == `RISCV_OP)
                            || (opcode == `RISCV_OP_IMM)
                            || (opcode == `RISCV_AUIPC);

    wire use_mem_result;
    assign use_mem_result = (opcode == `RISCV_LOAD);

    wire use_immediate;
    assign use_immediate = (opcode == `RISCV_LUI);

    wire use_pc;
    assign use_pc = (opcode == `RISCV_JALR || opcode == `RISCV_JAL);

    wire use_csr;
    assign use_csr = (opcode == `RISCV_SYSTEM)
                   && ((func3 == `RISCV_CSRRW)
                     | (func3 == `RISCV_CSRRS)
                     | (func3 == `RISCV_CSRRC)
                     | (func3 == `RISCV_CSRRWI)
                     | (func3 == `RISCV_CSRRSI)
                     | (func3 == `RISCV_CSRRCI));

    always @(*) begin
        if (use_alu_result) begin
            output_value = alu_result;
        end else if (use_mem_result) begin
            output_value = memory_result;
        end else if (use_immediate) begin
            output_value = immediate;
        end else if (use_pc) begin
            output_value = pc + 4;
        end else if (use_csr) begin
            output_value = csr_result;
        end
    end

    assign write_enable = (stage == `STAGE_REGISTER_UPDATE)
                        && (use_csr || use_pc || use_immediate || use_alu_result || use_mem_result);

endmodule
