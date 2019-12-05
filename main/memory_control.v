`ifndef ARCH_DEFINES
`define ARCH_DEFINES
`include "arch_defines.v"
`endif // ARCH_DEFINES

module memory_control(
        input rst,
        input clk,                   // the system's clock
        input start,                 // indiction of the start of a memory operation,
                                     // while high on a positive clock edge store all
                                     // input data into registers and start the
                                     // process
        input[31:0] address,         // the address to read/write
        input[2:0] mode,             // the mode of operation (aka the FUNC3 of the
                                     // load/store opcode
        input[31:0] write_data,      // if write_enable, this is the data to write
        output done,                 // high for one clock cycle to indicate that the
                                     // operation was done
        output[31:0] read_data,      // if not write_enable this is the data that was
                                     // read
        output active,               // high as long as the operation is going on
                                     // (from the clock's first pos edge to done)
        input[7:0] data_from_mem,    //
        output[31:0] address_to_mem, //
        output[7:0] data_to_mem      ///
    );

    reg[2:0] counter;
    reg[2:0] offset;
    reg[31:0] result_data_reg;
    reg first;
    reg active_reg;
    reg done_reg;

    always @(posedge clk) begin
        if (!rst) begin
            counter <= 0;
            offset <= 0;
            result_data_reg <= 0;
            first <= 0;
            active_reg <= 0;
            done_reg <= 0;
        end else if (start == 1) begin
            active_reg <= 1;
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
            if (first == 1) begin
                first <= 0;
                if (mode[2] == 0 && data_from_mem[7] == 1) begin
                    result_data_reg <= {24'hFFFFFF, data_from_mem};
                end else begin
                    result_data_reg <= {24'h000000, data_from_mem};
                end
            end else if (! done_reg ) begin
                result_data_reg <= (result_data_reg << 8) 
                                    | ({24'b0, data_from_mem});
            end
            if (offset == 0 && counter == 1) begin
                done_reg <= 1;
                active_reg <= 0;
            end
            if (counter == 0) begin
                done_reg <= 0;
                active_reg <= 0;
            end else begin
                counter <= counter - 1;
                offset <= offset - 1;
            end
        end
    end

    assign read_data = result_data_reg;
    assign active = active_reg;
    assign done = done_reg;
    assign address_to_mem = address + {28'b0, offset};
    assign data_to_mem = offset==3 ?
                        write_data[31:24] :
                        offset==2 ?
                            write_data[23:16] :
                            offset==1 ?
                                write_data[15:8] :
                                offset==0 ?
                                    write_data[7:0] :
                                    8'b0;
endmodule
