`ifndef ARCH_DEFINES
`define ARCH_DEFINES
`include "arch_defines.v"
`endif // ARCH_DEFINES

/*
 * Control module for the current instruction
 */
module current_instruction_register_control(
      input  `STAGE_WIDTH stage,
      output enable
   );

   assign enable = (stage == `STAGE_FETCH);
   
endmodule
