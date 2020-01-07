`default_nettype none

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
	 
    reg[31:0] counter = 32'd0;

    always @(posedge w_clk_low)
    begin
        if (rst) begin
            counter <= counter + 32'd1;
            if(counter>=255) begin
                counter <= 32'd0;
            end
        end
    end

    wire[7:0] w_data;
    out_port out_port(
        .write_data(counter),
        .write_enable(counter[2:0] == 3'b100),
        .rst(rst),
        .clk(clk),
        .port(w_data)
    );

    assign port[0] = w_data[0];
    assign port[1] = w_data[1];
    assign port[2] = w_data[2];
    assign port[3] = w_data[3];
    assign port[4] = w_data[4];
    assign port[5] = w_data[5];
    assign port[6] = w_data[6];
    assign port[7] = w_data[7];
    assign clk_out = 0;
endmodule