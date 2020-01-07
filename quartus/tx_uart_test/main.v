`default_nettype none

module main(
        input clk,
        input rst,
        output [7:0] port,
        output clk_out,
        output tx_line
    );
	 
    wire w_clk_low;
	wire w_locked;
	pll pll1(
		.inclk0(clk),
		.c0(w_clk_low),
		.locked(w_locked)
    );
	 
    reg[31:0] counter = 32'd0;
    reg[31:0] address = 32'd0;

    always @(posedge w_clk_low)
    begin
        if (rst) begin
            if (counter == 0) begin
                // set speed and active conf_enable

                w_config_data <= 8'b00001011; // 115200 Baud
                w_config_enable <= 1;
            end
            if (counter == 1) begin
                // disable conf_enable
                w_config_enable <= 0;
            end
            if (counter == 2) begin
                // read value is done by rom
            end
            if (counter == 3) begin
                // increase address and wrap at 16
                address <= address + 32'd1;
                if (address >= 15) begin
                    address <= 32'd0;
                end
            end
            if (counter == 4) begin
                // set write_data and activate write_enable
                w_write_data <= w_data;
                w_write_enable <= 1;
            end
            if (counter == 5) begin
                // disable write_enable
                w_write_enable <= 0;
            end
            // do nothing in cycle 6...876

            counter <= counter + 32'd1;
            if (counter >= 876) begin
                counter <= 32'd2;
            end
        end
    end

    wire[7:0] w_data;
    wire w_illegal;
    rom_memory rom(
        address,
        w_data,
        w_illegal
    );

    wire[7:0] w_write_data;
    wire w_write_enable;
    wire[7:0] w_config_data;
    wire w_config_enable;
    wire w_tx_line;
    tx_uart tx(
        .write_data(w_write_data),
        .write_enable(w_write_enable),
        .clk(clk),
        .config_data(w_config_data),
        .config_enable(w_config_enable),
        .tx_line(w_tx_line)
    );

    assign port[0] = 0; //w_data[0];
    assign port[1] = 0; //w_data[1];
    assign port[2] = 0; //w_data[2];
    assign port[3] = 0; //w_data[3];
    assign port[4] = 0; //w_data[4];
    assign port[5] = w_config_enable; // w_data[5];
    assign port[6] = w_write_enable; // w_data[6];
    assign port[7] = w_tx_line; //w_data[7];
    assign tx_line = w_tx_line; //w_data[7];
    assign clk_out = w_clk_low;
endmodule