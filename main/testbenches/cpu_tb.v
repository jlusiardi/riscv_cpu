`ifndef ARCH_DEFINES
`define ARCH_DEFINES
`include "../arch_defines.v"
`endif // ARCH_DEFINES

module cpu_tb;
    reg clk_tb;
    reg rst_tb;

    wire [2:0] stage_tb;

    cpu dut(
        .clk(clk_tb),
        .rst(rst_tb),
        .w_stage(stage_tb)
    );

    initial
    begin: CLOCK_GENERATOR
        clk_tb = 0;
        forever begin
            #5
            clk_tb = ~ clk_tb;
            if (clk_tb) begin
                $display("stage: %d", dut.stage_counter.data);
                if (dut.stage_counter.data == 1) begin
                    $display("command: %h", dut.w_current_instruction);
                end
                if (dut.stage_counter.data == 4) begin
                    $display("x1: %h", dut.register_file.registers[0]);
                end
            end
        end
    end

    initial begin
        $readmemh("../main/testbenches/rom.mem", dut.mem_ctrl.mem.rom.mem, 0);
        rst_tb = 0;

        #5
        #5
        #5
        #5
        rst_tb = 1;

    end

endmodule
