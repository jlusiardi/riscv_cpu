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

`define BAUD_50      4'b0000
`define BAUD_110     4'b0001
`define BAUD_150     4'b0010
`define BAUD_300     4'b0011
`define BAUD_1200    4'b0100
`define BAUD_2400    4'b0101
`define BAUD_4800    4'b0110
`define BAUD_9600    4'b0111
`define BAUD_19200   4'b1000
`define BAUD_38400   4'b1001
`define BAUD_57600   4'b1010
`define BAUD_115200  4'b1011
`define BAUD_230400  4'b1100
`define BAUD_460800  4'b1101
`define BAUD_500000  4'b1110
`define BAUD_MAX     4'b1111

module tx_uart(
        input[7:0] write_data,
        input write_enable,
        input clk,
        /* verilator lint_off UNUSED */
        // only the 4 lower bits are currently used.
        input[7:0] config_data,
        input config_enable,
        /* verilator lint_on UNUSED */
        output tx_line
    );

    reg[7:0] write_data_reg;
    reg tx_line_reg;
    reg[22:0] counter_reg;
    reg[7:0] state_reg;
    reg[22:0] cycles;

    initial begin
        tx_line_reg = 1;
        counter_reg = 23'd0;
        state_reg=`STOP_BIT;
        cycles = 2;
    end

    always @(posedge clk) begin
        if (counter_reg > 0) begin
            // keep everythin stationary for some cycles
            counter_reg <= counter_reg - 1;
        end else begin
            if (write_enable || config_enable) begin
                if (write_enable) begin
                    write_data_reg <= write_data;
                    tx_line_reg <= 1;
                    state_reg <= `START_BIT;
                    counter_reg <= cycles;
                end
                if (config_enable) begin
                    case (config_data[3:0])
                        `BAUD_50: begin
                            cycles <= 1000000;
                        end
                        `BAUD_110: begin
                            cycles <= 454545;
                        end
                        `BAUD_150: begin
                            cycles <= 333333;
                        end
                        `BAUD_300: begin
                            cycles <= 166666;
                        end
                        `BAUD_1200: begin
                            cycles <= 41666;
                        end
                        `BAUD_2400: begin
                            cycles <= 20833;
                        end
                        `BAUD_4800: begin
                            cycles <= 10416;
                        end
                        `BAUD_9600: begin
                            cycles <= 5208;
                        end
                        `BAUD_19200: begin
                            cycles <= 2604;
                        end
                        `BAUD_38400: begin
                            cycles <= 1302;
                        end
                        `BAUD_57600: begin
                            cycles <= 868;
                        end
                        `BAUD_115200: begin
                            cycles <= 434;
                        end
                        `BAUD_230400: begin
                            cycles <= 217;
                        end
                        `BAUD_460800: begin
                            cycles <= 108;
                        end
                        `BAUD_500000: begin
                            cycles <= 100;
                        end
                        `BAUD_MAX: begin
                            cycles <= 2;
                        end
                        default: begin
                            cycles <= 2;
                        end
                    endcase
                end
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
                    tx_line_reg <= ((write_data_reg & 8'b00000001 << state_reg) > 0) ? 1'b1 : 1'b0;
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
