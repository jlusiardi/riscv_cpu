`ifndef ARCH_DEFINES
`define ARCH_DEFINES
`include "arch_defines.v"
`endif // ARCH_DEFINES

module soc(
        input clk,
        input rst,
        output[2:0] leds
    );

    wire w_write_enable;
    wire[7:0] w_read_data;
    wire[31:0] w_address;
    wire[7:0] w_write_data;
	 
    cpu cpu(
        .clk(clk),
        .rst(rst),
        .w_stage(leds),
        .read_from_memory(w_read_data),
        .memory_address(w_address),
        .write_to_memory(w_write_data),
        .write_enable(w_write_enable)

    );

    memory mem(
        .read_data(w_read_data),
        .address(w_address),
        .write_data(w_write_data),
        .write_enable(w_write_enable),
        .clk(!clk)
    );
endmodule
