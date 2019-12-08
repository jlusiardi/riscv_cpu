`ifndef ARCH_DEFINES
`define ARCH_DEFINES
`include "arch_defines.v"
`endif // ARCH_DEFINES

module rom_memory(
        input [31:0] address,
        output [7:0] read_data,
        output illegal_address
    );

    /*
     * The rom must be initialized by the calling top modules or by the test!
     */
    /* verilator lint_off UNDRIVEN */
    (* ram_init_file = "../main/riscv_cpu.mif" *) reg [7:0] mem[`ROM_SIZE - 1 : 0];
    /* verilator lint_on UNDRIVEN */

    assign read_data = mem[address];
    assign illegal_address = (address >= `ROM_SIZE);

endmodule
