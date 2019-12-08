`ifndef ARCH_DEFINES
`define ARCH_DEFINES
`include "arch_defines.v"
`endif // ARCH_DEFINES

`define START_BIT   8'b11111111
`define BIT_0       8'b00000000
`define BIT_1       8'b00000001
`define BIT_2       8'b00000010
`define BIT_3       8'b00000011
`define BIT_4       8'b00000100
`define BIT_5       8'b00000101
`define BIT_6       8'b00000110
`define BIT_7       8'b00000111
`define STOP_BIT    8'b00001000


module tx_uart(
        input[7:0] write_data,
        input write_enable,
        input clk,
        input[27:0] cycles,
        output tx_line
    );

    reg[7:0] write_data_reg;
    reg tx_line_reg;
    reg[27:0] counter_reg;
    reg[7:0] state_reg;

    initial begin
        tx_line_reg = 1;
        counter_reg=28'd0;
        state_reg=`START_BIT;
    end

    always @(posedge clk) begin
        if (counter_reg > 0) begin
            // keep everythin stationary for some cycles
            counter_reg <= counter_reg - 1;
        end else begin
            if (write_enable) begin
                write_data_reg <= write_data;
                tx_line_reg <= 1;
                counter_reg <= cycles -1 ;
                state_reg <= `START_BIT;
            end else begin
                if (state_reg == `START_BIT) begin
                    tx_line_reg <= 0;
                    counter_reg <= cycles;
                    state_reg <= `BIT_0;
                end else if ((state_reg == `BIT_0) 
                        || (state_reg == `BIT_1) 
                        || (state_reg == `BIT_2) 
                        || (state_reg == `BIT_3) 
                        || (state_reg == `BIT_4) 
                        || (state_reg == `BIT_5) 
                        || (state_reg == `BIT_6) 
                        || (state_reg == `BIT_7)) begin
                    tx_line_reg <= ((write_data_reg & 8'b00000001 << state_reg) > state_reg) ? 1'b1 : 1'b0;
                    counter_reg <= cycles;
                    state_reg <= state_reg + 1;
                end else if (state_reg == `STOP_BIT) begin
                    tx_line_reg <= 1;
                end
            end
        end
    end

    assign tx_line = tx_line_reg;
endmodule
