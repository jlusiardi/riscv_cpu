`ifndef ARCH_DEFINES
`define ARCH_DEFINES
`include "arch_defines.v"
`endif // ARCH_DEFINES

module stage_counter(
        input clk,
        input  rst,
        output [2:0] out
    );
   
    reg [2:0] data;

    always @(posedge clk or negedge rst) begin
        if (!rst) begin
            data <= 0;
        end else begin
            if (data == (`NUM_STAGES - 1)) begin
                data <= 0;
            end else begin
                data <= data + 1'b1;
            end
         //$display("                   Stage = 0x%h", data);
        end
    end
   
    assign out = data;
   
endmodule
