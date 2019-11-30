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

    initial begin
        data = `STAGE_RESET;
        start_fetch = 0;
        start_memory = 0;
    end

    always @(posedge clk or negedge rst) begin
        if (start_fetch == 1)
            start_fetch <= 0;
        if (start_memory == 1)
            start_memory <= 0;
        if (!rst) begin
            data <= `STAGE_RESET;
            start_fetch <= 0;
            start_memory <= 0;
        end else if (!blocked) begin
            if (data == (`NUM_STAGES - 1) || data == `STAGE_RESET) begin
                start_fetch <= 1;
                data <= 0;
            end else begin
                if (data == `STAGE_EXECUTE)
                    start_memory <= 1;
                data <= data + 1'b1;
            end
        end
    end
   
    assign out = data;
   
endmodule
