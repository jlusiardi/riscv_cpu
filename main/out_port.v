`ifndef ARCH_DEFINES
`define ARCH_DEFINES
`include "arch_defines.v"
`endif // ARCH_DEFINES

module out_port(
        input[7:0] write_data,
        input write_enable,
        input rst,
        input clk,
        output[7:0] out_lines
    );

    reg[7:0] write_data_reg;

    always @(posedge clk) begin
        if (!rst) begin
            write_data_reg <= 8'b0;
        end else if (write_enable) begin
            write_data_reg <= write_data;
        end
    end

    assign out_lines = write_data_reg;
endmodule
