`ifndef ARCH_DEFINES
`define ARCH_DEFINES
`include "arch_defines.v"
`endif // ARCH_DEFINES

module memory_control_synth(
        input clk,
        input start,
        input[31:0] address,
        input[2:0] mode,
        input write_enable,
        input[31:0] write_data,
        output done,
        output[31:0] read_data
    );

    reg[2:0] counter;
    reg[2:0] offset;
    reg[31:0] address_reg;
    wire[7:0] read_data_reg;
    reg[31:0] result_data_reg;
    reg[31:0] write_all_data_reg;
    reg first;
    reg active;
    reg done_reg;
    reg write_enable_reg;

    memory_synth mem ( 
        .read_data(read_data_reg),
        .address(address_reg + {28'b0, offset}),
        .write_data(offset==3 ?
                        write_all_data_reg[31:24] :
                        offset==2 ?
                            write_all_data_reg[23:16] :
                            offset==1 ?
                                write_all_data_reg[15:8] :
                                offset==0 ?
                                    write_all_data_reg[7:0] :
                                    8'b0
                    ),
        .write_enable(write_enable_reg),
        .clk(clk)
    );

    always @(posedge clk) begin
        if (start == 1) begin
            address_reg <= address;
            write_enable_reg <= write_enable;
            write_all_data_reg <= write_data;
            active <= 1;
            first <= 1;
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
                    done_reg <= 1;
                    write_enable_reg <= 0;
                end;
                if (counter == 0) begin
                    done_reg <= 0;
                    result_data_reg <= 32'b0;
                end else begin
                    counter <= counter - 1;
                    offset <= offset - 1;
                end
            end
        end
    end

    assign read_data = result_data_reg;
    assign done = done_reg;

endmodule
