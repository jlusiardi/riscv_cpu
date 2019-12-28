module main(
        input clk,
        input rst,
        output [7:0] port,
        output clk_out
    );
	 
    wire w_clk_low;
	wire w_locked;
	pll pll1(
		.inclk0(clk),
		.c0(w_clk_low),
		.locked(w_locked)
    );
	 
    reg[7:0]counter = 8'd0;
    always @(posedge w_clk_low)
    begin
        counter <= counter + 8'd1;
        if(counter>=255) begin
            counter <= 8'd0;
        end
    end

    assign port[0] = counter[0];
    assign port[1] = counter[1];
    assign port[2] = counter[2];
    assign port[3] = counter[3];
    assign port[4] = counter[4];
    assign port[5] = counter[5];
    assign port[6] = counter[6];
    assign port[7] = counter[7];
    assign clk_out = w_clk_low;
endmodule