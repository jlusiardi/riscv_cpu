`ifndef ARCH_DEFINES
`define ARCH_DEFINES
`include "arch_defines.v"
`endif // ARCH_DEFINES

module soc(
        input clk,
        input rst,
        output[2:0] leds
    );

    wire clk_low;
    clock_divider div(
        .clock_in(clk),
        .clock_out(clk_low)
    );
	 
    cpu dut(
        .clk(clk_low),
        .rst(rst),
        .w_stage(leds)
    );

endmodule
