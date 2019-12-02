`ifndef ARCH_DEFINES
`define ARCH_DEFINES
`include "arch_defines.v"
`endif // ARCH_DEFINES

module stage_counter_synth(
        input clk,
        input rst,
        input blocked,
        output [2:0] out,
        output start_fetch,
        output start_memory
    );
   
    reg [2:0] data;
    reg start_fetch_reg;
    reg start_memory_reg;

    initial begin
        data = `STAGE_RESET;
        start_fetch_reg = 0;
        start_memory_reg = 0;
    end

    always @(posedge clk) begin
        if (start_fetch_reg)
            start_fetch_reg <= 0;
        if (start_memory_reg)
            start_memory_reg <= 0;
        if (!rst) begin
            data <= `STAGE_RESET;
            start_memory_reg <= 0;
            start_memory_reg <= 0;
        end else if (!blocked) begin
            if (data == (`NUM_STAGES - 1) || data == `STAGE_RESET) begin
                start_fetch_reg <= 1;
                data <= 0;
            end else begin
                if (data == `STAGE_EXECUTE)
                    start_memory_reg <= 1;
                data <= data + 1'b1;
            end
        end
    end
   
    assign out = data;
    assign start_fetch = start_fetch_reg;
    assign start_memory = start_memory_reg;
endmodule
