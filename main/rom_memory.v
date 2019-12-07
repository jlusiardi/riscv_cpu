module rom_memory(
        input output_enable,
        input [31:0] address,
        output [7:0] read_data,
        output illegal_address
    );

    parameter depth = 512;

    /*
     * The rom must be initialized by the calling top modules or by the test!
     */
    /* verilator lint_off UNDRIVEN */
    (* ram_init_file = "../main/riscv_cpu.mif" *) reg [7:0] mem[4 * depth - 1 : 0];
    /* verilator lint_on UNDRIVEN */

    assign read_data = output_enable ?
                         mem[address] :
                         8'hZ;
    assign illegal_address = (address >= 4 * depth);

endmodule
