module register_file(
        input rst,
        input [4:0] read_address_0,
        output [31:0] read_data_0,
        input [4:0] read_address_1,
        output [31:0] read_data_1, 
        input [4:0] write_address,
        input [31:0] write_data,
        input write_enable,
        input clk
    );

    reg [31:0] registers[30:0];

    integer i;

    initial begin
        for (i=0;i<=30;i=i+1) begin
            registers[i] = 0;
        end
    end

    always @(posedge clk) begin
        if (!rst) begin
            for (i=0;i<31;i=i+1) begin
                registers[i] <= 0;
            end
        end else if (write_enable) begin
            if (write_address != 0)
                //$display("     Writing to register = 0x%h (Register 0x%h)\n", write_data, write_address);
                registers[write_address - 1] <= write_data;
        end
    end

    assign read_data_0 = read_address_0==0 ? 32'b0 : registers[read_address_0 - 1] ;
    assign read_data_1 = read_address_1==0 ? 32'b0 : registers[read_address_1 - 1];
endmodule
