`ifndef ARCH_DEFINES
`define ARCH_DEFINES
`include "arch_defines.v"
`endif // ARCH_DEFINES

module synth_soc(
        input clk,
        input rst,
        output[2:0] leds,
        output[7:0] data
    );

    wire clk_low;
    wire w_write_enable;
    wire[7:0] w_read_data;
    wire[31:0] w_address;
    wire[7:0] w_write_data;

    clock_divider div(
        .clk_in(clk),
        .clk_out(clk_low)
    );

    cpu cpu(
        .clk(clk_low),
        .rst(rst),
        .w_stage(leds),
        .read_from_memory(w_read_data),
        .memory_address(w_address),
        .write_to_memory(w_write_data),
        .write_enable(w_write_enable)

    );

    rom_memory rom (
        .output_enable(
            $signed(rom_start) <= $signed(w_address)
                && $signed(w_address) < $signed(rom_start + rom_size) ?
            0'b1 :
            0'b0
        ),
        .address(w_address - rom_start),
        .read_data(w_read_data),
        .illegal_address(rom_illegal_address)
    );

    ram_memory ram (
        .output_enable(
            $signed(ram_start) <= $signed(w_address)
                && $signed(w_address) < $signed(ram_start + ram_size) ?
            0'b1 :
            0'b0
        ),
        .read_data(w_read_data),
        .address(w_address - ram_start),
        .write_data(w_write_data),
        .write_enable(w_write_enable),
        .illegal_address(ram_illegal_address),
        .clk(clk)
    );

    assign data = w_write_data;
endmodule
