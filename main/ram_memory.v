`ifndef ARCH_DEFINES
`define ARCH_DEFINES
`include "arch_defines.v"
`endif // ARCH_DEFINES

/*
 * Memory for 1024 32bit words 
 * Memory is used for instructions and data
 */
module ram_memory(
        input [31:0] read_address,
        input [2:0] size_and_sign,
        output [31:0] read_data,
        output illegal_write_address,
        output illegal_read_address,

        input [31:0] write_address,
        input [31:0] write_data,

        input write_enable,

        input clk
    );

    parameter depth = 1024;

    reg [7:0] mem[4 * depth - 1 : 0];

    always @(posedge clk) begin

        if (write_enable) begin
            case (size_and_sign)
                `STORE_BYTE: begin
                    mem[write_address + 0] <= write_data[ 7: 0];
                end
                `STORE_HALFWORD: begin
                    mem[write_address + 1] <= write_data[15: 8];
                    mem[write_address + 0] <= write_data[ 7: 0];
                end
                `STORE_WORD: begin
                    mem[write_address + 3] <= write_data[31:24];
                    mem[write_address + 2] <= write_data[23:16];
                    mem[write_address + 1] <= write_data[15: 8];
                    mem[write_address + 0] <= write_data[ 7: 0];
                end
                default: begin
                    // empty by intention
                end
            endcase
            illegal_write_address <= (write_address >= depth);
        end
      
    end

    assign read_data = {mem[read_address+3], mem[read_address + 2], mem[read_address + 1], mem[read_address + 0]};

    assign illegal_read_address = (read_address >= 4 * depth);

endmodule
