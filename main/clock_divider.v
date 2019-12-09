
module clock_divider(
        input clk_in,
        output clk_out
    );

    /*
     * The f(clk_out) = f(clk_in) / DIVISOR
     */
    parameter DIVISOR = 28'd5000000;

    reg[27:0] counter=28'd0;
    always @(posedge clk_in)
    begin
        counter <= counter + 28'd1;

        if(counter>=(DIVISOR-1)) begin
            counter <= 28'd0;
        end
    end

    assign clk_out = (counter < DIVISOR/2 ) ? 1'b0 : 1'b1;
endmodule

