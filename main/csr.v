`ifndef ARCH_DEFINES
`define ARCH_DEFINES
`include "arch_defines.v"
`endif // ARCH_DEFINES

module csr(
        input [2:0] operation,
        input [11:0] csr_number,
        input [4:0] rs1,
        input [31:0] input_value,
        output [31:0] csr_value,
        input write_enable,
        input clk
    );

    reg [31:0] csrs[4095 : 0];
    wire csr_ro = csr_number[11:10] == 2'b11;

    initial begin
        // CSR_MISA => 0x301
        csrs[769] = 32'b01000000000000000000000100000000;
    end

    always @(posedge clk) begin
        if (write_enable & !csr_ro) begin
            case (operation)
                `RISCV_CSRRW, `RISCV_CSRRWI: begin
                    // csr_value = csrs[csr_number]; csrs[csr_number] = input_value;
                    csr_value <= csrs[csr_number];
                    if (rs1 != 0) begin
                        csrs[csr_number] <= input_value;
                    end
                end
                `RISCV_CSRRS, `RISCV_CSRRSI: begin
                    // csr_value = csrs[csr_number]; csrs[csr_number] = input_value;
                    csr_value <= csrs[csr_number];
                    if (rs1 != 0) begin
                        csrs[csr_number] <= csrs[csr_number] | input_value;
                    end
                end
                `RISCV_CSRRC, `RISCV_CSRRCI: begin
                    csr_value <= csrs[csr_number];
                    if (rs1 != 0) begin
                        csrs[csr_number] <= csrs[csr_number] & (~input_value);
                    end
                end
                default: begin            
                end
            endcase
        end
    end

endmodule
