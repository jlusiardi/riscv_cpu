`ifndef ARCH_DEFINES
`define ARCH_DEFINES
`include "../arch_defines.v"
`endif // ARCH_DEFINES

module stage_counter_synth_tb;
    reg clk_tb;
    reg rst_tb;
    reg blocked_tb;

    wire [2:0] out_tb;
    wire start_fetch_tb;
    wire start_memory_tb;

    stage_counter_synth stage_counter(
        .clk(clk_tb),
        .rst(rst_tb),
        .blocked(blocked_tb),
        .out(out_tb),
        .start_fetch(start_fetch_tb),
        .start_memory(start_memory_tb)
    );

    initial
    begin: CLOCK_GENERATOR
        clk_tb = 0;
        forever begin
            #5 clk_tb = ~ clk_tb;
        end        
    end

    initial begin
        rst_tb = 0;
        blocked_tb = 0;

        #20
        rst_tb = 1;
    end

endmodule
