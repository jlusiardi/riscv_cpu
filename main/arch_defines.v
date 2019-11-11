
`define STAGE_FETCH 0
`define STAGE_DECODE 1
`define STAGE_EXECUTE 2
`define STAGE_MEMORY 3
`define STAGE_REGISTER_UPDATE 4

`define NUM_STAGES 5
`define STAGE_WIDTH [2:0]

`define RISCV_LUI           7'b0110111
`define RISCV_AUIPC         7'b0010111
`define RISCV_JAL           7'b1101111
`define RISCV_JALR          7'b1100111
`define RISCV_ALU_OP_REGS   7'b0110011  // ALU ops directly operating on 3 registers
`define RISCV_ALU_OP_IMM    7'b0010011  // ALU ops directly operating on 2 registers & immediate
`define RISCV_LOAD          7'b0000011  // like INSTR_LOAD
`define RISCV_STORE         7'b0100011  // like INSTR_STORE
`define RISCV_BRANCH        7'b1100011  // like INSTR_JUMP

`define RISCV_BEQ           3'b000
`define RISCV_BNE           3'b001
`define RISCV_BLT           3'b100
`define RISCV_BGE           3'b101
`define RISCV_BLTU          3'b110
`define RISCV_BGEU          3'b111

`define RISCV_ADD           3'b000
`define RISCV_SLL           3'b001
`define RISCV_XOR           3'b100
`define RISCV_OR            3'b110
`define RISCV_AND           3'b111
`define RISCV_SRL           3'b101
`define RISCV_SLT           3'b010
`define RISCV_SLTU          3'b011

`define LOAD_SIGNED_BYTE       3'b000
`define STORE_BYTE             3'b000
`define LOAD_UNSIGNED_BYTE     3'b100
`define LOAD_SIGNED_HALFWORD   3'b001
`define STORE_HALFWORD         3'b001
`define LOAD_UNSIGNED_HALFWORD 3'b101
`define LOAD_WORD              3'b010
`define STORE_WORD             3'b010

`define RISCV_STD_OP        7'b0000000
`define RISCV_ALT_OP        7'b0100000

