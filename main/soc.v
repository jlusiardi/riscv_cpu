`ifndef ARCH_DEFINES
`define ARCH_DEFINES
`include "arch_defines.v"
`endif // ARCH_DEFINES

module soc(
        input clk,
        input rst,
        output[2:0] leds,
        output tx_line,
        output[7:0] port
    );

    /* verilator lint_off UNUSED */
    wire rom_illegal_address;
    wire ram_illegal_address;
    /* verilator lint_on UNUSED */
    wire w_write_enable;
    wire[7:0] w_write_data;
    wire[7:0] w_read_data;
    wire[7:0] w_rom_read_data;
    wire[7:0] w_ram_read_data;
    wire[31:0] w_address;

    cpu cpu(
        .clk(clk),
        .rst(rst),
        .w_stage(leds),
        .read_from_memory(w_read_data),
        .memory_address(w_address),
        .write_to_memory(w_write_data),
        .write_enable(w_write_enable)

    );

    rom_memory rom (
        .address(w_address - `ROM_START),
        .read_data(w_rom_read_data),
        .illegal_address(rom_illegal_address)
    );

    ram_memory ram (
        .read_data(w_ram_read_data),
        .address(w_address - `RAM_START),
        .write_data(w_write_data),
        .write_enable(w_write_enable),
        .illegal_address(ram_illegal_address),
        .clk(!clk)
    );

    tx_uart tx_uart(
        .write_data(w_write_data),
        .write_enable(w_address == `UART_SEND),
        .clk(clk),
        .config_data(w_write_data),
        .config_enable(w_address == `UART_CONFIG),
        .tx_line(tx_line)

    );

    out_port outport(
        .rst(rst),
        .write_data(w_write_data),
        .write_enable(w_address == `OUT_PORT),
        .clk(clk),
        .port(port)
    );

    assign w_read_data =
                        ($signed(`RAM_START) <= $signed(w_address) && $signed(w_address) < $signed(`RAM_START + `RAM_SIZE)
                            ? w_ram_read_data : 8'b0 )
                        | ($signed(`ROM_START) <= $signed(w_address) && $signed(w_address) < $signed(`ROM_START + `ROM_SIZE)
                            ? w_rom_read_data : 8'b0 );

endmodule
