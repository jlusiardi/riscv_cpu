`ifndef ARCH_DEFINES
`define ARCH_DEFINES
`include "arch_defines.v"
`endif // ARCH_DEFINES

module stage_counter(
        input clk,
        input rst,
        input blocked,
        output [2:0] stage,
        output start_fetch,
        output start_memory
    );
   
    reg [2:0] stage_reg;
    reg start_fetch_reg;
    reg start_memory_reg;

    initial begin
        stage_reg = `STAGE_RESET;
        start_fetch_reg = 0;
        start_memory_reg = 0;
    end

    always @(posedge clk) begin
        if (start_fetch_reg) begin
            // start_fetch_reg should be high for one cycle
            start_fetch_reg <= 0;
        end
        if (start_memory_reg) begin
            // start_memory_reg should be high for one cycle
            start_memory_reg <= 0;
        end
        if (!rst) begin
            stage_reg <= `STAGE_RESET;
            start_fetch_reg <= 0;
            start_memory_reg <= 0;
        end else if (!blocked) begin
            if (stage_reg == `STAGE_RESET) begin
                start_fetch_reg <= 1;
                stage_reg <= `STAGE_FETCH;
            end else if (stage_reg == `STAGE_FETCH) begin
                stage_reg <= `STAGE_DECODE;
            end else if (stage_reg == `STAGE_DECODE) begin
                stage_reg <= `STAGE_EXECUTE;
            end else if (stage_reg == `STAGE_EXECUTE) begin
                stage_reg <= `STAGE_MEMORY;
                start_memory_reg <= 1;
            end else if (stage_reg == `STAGE_MEMORY) begin
                stage_reg <= `STAGE_REGISTER_UPDATE;
            end else if (stage_reg == `STAGE_REGISTER_UPDATE) begin
                start_fetch_reg <= 1;
                stage_reg <= `STAGE_FETCH;
            end
        end
    end
   
    assign stage = stage_reg;
    assign start_fetch = start_fetch_reg;
    assign start_memory = start_memory_reg;
endmodule
