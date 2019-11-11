module rom_memory(
        input [31:0]  read_address,
        output [31:0] read_data,
        output illegal_read_address
    );

    parameter depth = 512;

    /*
     * The rom must be initialized by the calling top modules or by the test!
     */
    /* verilator lint_off UNDRIVEN */
    reg [7:0] rom[4 * depth - 1 : 0];

    assign read_data = {rom[read_address+3], rom[read_address + 2], rom[read_address + 1], rom[read_address + 0]};
    assign illegal_read_address = (read_address >= 4 * depth);

endmodule
