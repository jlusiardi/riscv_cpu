`ifndef ARCH_DEFINES
`define ARCH_DEFINES
`include "arch_defines.v"
`endif // ARCH_DEFINES

/*
    rom @ 0x00000000 - 0x00000200
    ram @ 0x00000400 - 0x00000800
 */
module memory(
        input [31:0] read_address,
        output [31:0] read_data,
        output illegal_write_address,
        output illegal_read_address,
        input [2:0] size_and_sign,
        input [31:0] write_address,
        input [31:0] write_data,

        input write_enable,

        input clk);


    wire [31:0] ram_read_data;
    wire [31:0] rom_read_data;
    wire ram_illegal_write_address;
    wire ram_illegal_read_address;
    wire rom_illegal_read_address;

    rom_memory rom_(
        .read_address(read_address),
        .read_data(rom_read_data),
        .illegal_read_address(rom_illegal_read_address)
    );

    ram_memory ram(
        .read_address(read_address - 4096),
        .size_and_sign(size_and_sign),
        .read_data(ram_read_data),
        .illegal_read_address(ram_illegal_read_address),
        .illegal_write_address(ram_illegal_write_address),
        .write_address(write_address - 4096),
        .write_data(write_data),
        .write_enable(write_enable),
        .clk(clk)
    );

    always @(*) begin
        if (rom_illegal_read_address == 0)
            read_data = rom_read_data;
        else if (ram_illegal_read_address == 0) begin
            case (size_and_sign)
                `LOAD_SIGNED_BYTE:
                    read_data = {{25{ram_read_data[7]}}, ram_read_data[6:0]};
                `LOAD_UNSIGNED_BYTE:
                    read_data = {24'b0, ram_read_data[7:0]};
                `LOAD_SIGNED_HALFWORD:
                    read_data = {{17{ram_read_data[15]}}, ram_read_data[14:0]};
                `LOAD_UNSIGNED_HALFWORD:
                    read_data = {16'b0, ram_read_data[15:0]};
                `LOAD_WORD:
                    read_data = ram_read_data;
                default:
                    read_data = 32'b0;
            endcase
        end else begin
            // TODO dO nothing on intention and ignore quartus warning (should 
            // be fixed by adding something like read_enable?
            //read_data = 32'b0;
        end

        illegal_read_address = ram_illegal_read_address & rom_illegal_read_address;

        illegal_write_address = ram_illegal_write_address;
    end

endmodule
