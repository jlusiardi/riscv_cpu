`ifndef ARCH_DEFINES
`define ARCH_DEFINES
`include "arch_defines.v"
`endif // ARCH_DEFINES

module memory_control_synth(
        input clk,
        input start,
        input[31:0] address,
        /* verilator lint_off UNUSED */
        input[2:0] mode,
        /* verilator lint_on UNUSED */
        output done,
        output[31:0] read_data
    );

    reg[2:0] counter;
    reg[2:0] offset;
    reg[31:0] address_reg;
    /* verilator lint_off UNUSED */
    /* verilator lint_off UNDRIVEN */
    reg[7:0] read_data_reg;
    reg[31:0] result_data_reg;
    reg[7:0] write_data_reg;
    reg first;
    /* verilator lint_on UNDRIVEN */
    /* verilator lint_on UNUSED */
    reg active;

    
    memory_synth mem ( 
        .read_data(read_data_reg),
        .address(address_reg + {28'b0, offset}),
        .write_data(write_data_reg),
        .write_enable(1'b0),
        .clk(clk)
    );

    always @(posedge clk) begin
        if (start == 1) begin
            address_reg <= address;
            active <= 1;
            first <=1;
            if (mode[1:0] == 2'b0) begin
                counter <= 1;
                offset <= 0;
            end else if (mode[1:0] == 2'b01) begin
                counter <= 2;
                offset <= 1;
            end else begin
                counter <= 4;
                offset <= 3;
            end
        end else begin
            if (active == 1) begin
                if (first == 1) begin
                    first <= 0;
                    if (mode[2] == 0 && read_data_reg[7] == 1) begin
                        result_data_reg <= {24'hFFFFFF, read_data_reg};
                    end else begin
                        result_data_reg <= {24'h000000, read_data_reg};
                    end;
                end else begin
                    result_data_reg <= (result_data_reg << 8) 
                                     | ({24'b0, read_data_reg});
                end
                if (offset == 0 && counter == 1) begin
                    done <= 1;
                end;
                if (counter == 0) begin
                    done <= 0;
                    result_data_reg <= 32'b0;
                end else begin
                    counter <= counter - 1;
                    offset <= offset - 1;
                end
            end
        end
    end

    assign read_data = result_data_reg;

endmodule
