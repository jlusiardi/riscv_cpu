module reg_async_reset(
        input [31:0] D,
        output [31:0] Q,
        input en,
        input clk,
        input rst
    );

    reg [31: 0] Q_data;

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
