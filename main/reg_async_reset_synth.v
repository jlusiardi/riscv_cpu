module reg_async_reset_synth(
        input [width - 1:0] D,
        output [width - 1:0] Q,
        input en,
        input clk,
        input rst
    );

    parameter width = 32;

    reg [width - 1: 0] Q_data;

    always @(posedge clk) begin
        if (!rst) begin
            Q_data <= 0;
        end else begin
            if (en) begin
                Q_data <= D;
            end
        end
    end

    assign Q = Q_data;
   
endmodule
