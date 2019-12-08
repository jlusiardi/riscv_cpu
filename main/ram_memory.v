`ifndef ARCH_DEFINES
`define ARCH_DEFINES
`include "arch_defines.v"
`endif // ARCH_DEFINES

module ram_memory(
        read_data,
        address,
        write_data,
        write_enable,
        illegal_address,
        clk
    );

    output[7:0] read_data;
    input [7:0] write_data;
    input [31:0] address;
    input write_enable;
    input clk;
    output illegal_address;

    reg [7:0] read_data;

    reg [7:0] mem[`RAM_SIZE - 1 : 0];

    always @(posedge clk) begin
        if (write_enable)
            mem[address] <= write_data;
        read_data <= mem[address];
    end

    assign illegal_address = (address >= `RAM_SIZE);

endmodule
