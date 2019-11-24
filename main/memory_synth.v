`ifndef ARCH_DEFINES
`define ARCH_DEFINES
`include "arch_defines.v"
`endif // ARCH_DEFINES

/*
    rom @ 0x00000000 - 0x00000800
    ram @ 0x00001000 - 0x00003000
 */
module memory_synth(
        output[7:0] read_data,
        input[31:0] address,
        input[7:0] write_data,
        input write_enable,
        input clk
    );

    parameter rom_start = 32'h00000000;
    parameter ram_start = 32'h00001000;

    wire [7:0] rom_read_data;
    wire rom_illegal_address;

    rom_memory_synth rom (
        .address(address - rom_start),
        .read_data(rom_read_data),
        .illegal_address(rom_illegal_address)
    );

    wire [7:0] ram_read_data;
    /* verilator lint_off UNUSED */
    wire ram_illegal_address;
    /* verilator lint_on UNUSED */

    ram_memory_synth ram (
        .read_data(ram_read_data),
        .address(address - ram_start),
        .write_data(write_data),
        .write_enable(write_enable),
        .illegal_address(ram_illegal_address),
        .clk(clk)
    );

    reg[7:0] read_data_reg;

    always @(*) begin
        if (rom_illegal_address == 0) 
            read_data_reg = rom_read_data;
        else if (ram_illegal_address == 0) 
            read_data_reg = ram_read_data;
        else
            read_data_reg = 8'b0;
    end

    assign read_data = read_data_reg;
endmodule
