`ifndef ARCH_DEFINES
`define ARCH_DEFINES
`include "arch_defines.v"
`endif // ARCH_DEFINES

module cpu_synth(
        input clk,
        input rst
    );

    wire [2:0] w_stage;

    stage_counter_synth stage_counter(
        .clk(clk),
        .rst(rst),
        .blocked(w_blocking_mem),
        .out(w_stage)
    );

    wire w_current_instruction_register_write_enable;
    wire [31:0] w_current_instruction;

    current_instruction_register_control current_instruction_register_control(
        .stage(w_stage),
        .enable(w_current_instruction_register_write_enable)
    );

    reg_async_reset current_instruction_register(
        .clk(clk),
        .rst(rst),
        .en(w_current_instruction_register_write_enable),
        .D(w_read_data),
        .Q(w_current_instruction)
    );

    wire [2:0] w_func3;
    wire [6:0] w_func7;
    wire [31:0] w_immediate;
    /* verilator lint_off UNUSED */
    wire w_instr_valid;
    /* verilator lint_on UNUSED */
    wire [4:0] w_reg_src_0_addr;
    wire [4:0] w_reg_src_1_addr;
    wire [4:0] w_reg_dest_addr;
    
    decoder instruction_decoder(
        .instruction(w_current_instruction),
        .opcode(w_opcode),
        .func3(w_func3),
        .func7(w_func7),
        .imm(w_immediate),
        .instr_valid(w_instr_valid),
        .reg_source_0(w_reg_src_0_addr),
        .reg_source_1(w_reg_src_1_addr),
        .reg_dest(w_reg_dest_addr)
    );

    wire w_jump_condition;
    wire [31:0] w_pc_input;
    wire w_pc_write_enable;

    pc_control pc_control(
        .opcode(w_opcode),
        .stage(w_stage),
        .jump_condition(w_jump_condition),
        .offset(w_immediate),
        .rs1(w_register_src_0_register),
        .pc_input(w_pc_output),
        .pc_output(w_pc_input),
        .pc_en(w_pc_write_enable)
    );

    reg_async_reset pc_register(
        .clk(clk),
        .rst(rst),
        .en(w_pc_write_enable),
        .D(w_pc_input),
        .Q(w_pc_output)
    );

    wire [6:0] w_opcode;
    wire [31:0] w_pc_output;
    wire w_blocking_mem;
    /* verilator lint_off UNUSED */
    wire w_done;
    /* verilator lint_on UNUSED */

    memory_control_synth mem_ctrl(
        .clk(clk),
        .start(
            w_stage == `STAGE_FETCH
            || (w_stage == `STAGE_MEMORY && (w_func7 == `RISCV_LOAD
                                            || w_func7 == `RISCV_STORE))
        ),
        .address(
            w_pc_output
        ),
        .mode(w_func3),
        .write_enable(w_stage == `STAGE_MEMORY && w_func7 == `RISCV_STORE),
        .write_data(w_register_src_1_register),
        .done(w_done),
        .read_data(w_read_data),
        .active(w_blocking_mem)
    );

/*
    memory_control mem_ctrl(
      .stage(w_stage),
      .opcode(w_opcode),
      .pc_value(w_pc_output),
      .rs1_value(w_register_src_0_register),
      .offset(w_immediate),
      .memory_write_data(w_register_src_1_register),

      // Outputs to send to main memory
      .read_address(w_read_address),
      .write_address(w_write_address),
      .write_data(w_write_data),
      .write_enable(w_write_enable)
   );
*/
//    wire [31:0] w_read_address;
    wire [31:0] w_read_data;
    // TODO what should happen, if those go high?
    /* verilator lint_off UNUSED */
    wire w_illegal_write_address;
    wire w_illegal_read_address;
    /* verilator lint_on UNUSED */
//    wire [31:0] w_write_address;
//    wire [31:0] w_write_data;
/*
    wire w_write_enable;
    memory mem(
        .read_address(w_read_address),
        .read_data(w_read_data),
        .illegal_write_address(w_illegal_write_address),
        .illegal_read_address(w_illegal_read_address),
        .size_and_sign(w_func3),
        .write_address(w_write_address),
        .write_data(w_write_data),
        .write_enable(w_write_enable),
        .clk(clk)
    );
*/
    wire [31:0] w_register_src_0_register_int;
    wire [31:0] w_register_src_0_register;
    wire [31:0] w_register_src_1_register_int;
    wire [31:0] w_register_src_1_register;
    wire w_reg_file_write_en;
    wire [31:0] w_register_dest_register;

    register_file register_file(
        .read_address_0(w_reg_src_0_addr),
        .read_data_0(w_register_src_0_register_int),
        .read_address_1(w_reg_src_1_addr),
        .read_data_1(w_register_src_1_register_int),
        .write_address(w_reg_dest_addr),
        .write_data(w_register_dest_register),
        .write_enable(w_reg_file_write_en),
        .clk(clk)
    );

    reg_async_reset register_src_0_register(
        .clk(clk),
        .rst(rst),
        .en(1'b1),
        .D(w_register_src_0_register_int),
        .Q(w_register_src_0_register)
    );

    reg_async_reset register_src_1_register(
        .clk(clk),
        .rst(rst),
        .en(1'b1),
        .D(w_register_src_1_register_int),
        .Q(w_register_src_1_register)
    );

    wire [31:0] w_alu_in1;
    wire [31:0] w_alu_in2;
    alu_control alu_control(
        .opcode(w_opcode),
        .rs1(w_register_src_0_register),
        .rs2(w_register_src_1_register),
        .pc_value(w_pc_output),
        .immediate(w_immediate),
        .alu_in1(w_alu_in1),
        .alu_in2(w_alu_in2)
    );

    wire [31:0] w_alu_result;
    alu alu(
        .rs1(w_alu_in1),
        .rs2(w_alu_in2),
        .opcode(w_opcode),
        .func3(w_func3),
        .func7(w_func7),
        .out(w_alu_result)
    );

    register_file_control register_file_control(
        .stage(w_stage),
        .opcode(w_opcode),
        .alu_result(w_alu_result),
        .immediate(w_immediate),
        .memory_result(w_read_data),
        .pc(w_pc_output),
        .write_enable(w_reg_file_write_en),
        .output_value(w_register_dest_register)
    );

    compare_unit compare_unit(
        .rs1_value(w_register_src_0_register_int),
        .rs2_value(w_register_src_1_register_int),
        .cmp_op(w_func3),
        .jump_condition(w_jump_condition)
    );

endmodule
