module ram_memory_synth(
        read_data,
        address,
        write_data,
        write_enable,
        illegal_address,
        clk
    );

    output[7:0] read_data;
    input [7:0] write_data;
    /* verilator lint_off UNUSED */
    input [31:0] address;
    /* verilator lint_on UNUSED */
    input write_enable;
    input clk;
    output illegal_address;

    reg [7:0] read_data;
    parameter depth = 2048;

    reg [7:0] mem[4 * depth - 1 : 0];

    always @(posedge clk) begin
        if (write_enable)
            mem[address] <= write_data;
        read_data <= mem[address];
    end
    assign illegal_address = (address >= 4 * depth);

endmodule
