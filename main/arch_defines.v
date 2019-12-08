
`define STAGE_FETCH             0
`define STAGE_DECODE            1
`define STAGE_EXECUTE           2
`define STAGE_MEMORY            3
`define STAGE_REGISTER_UPDATE   4
`define STAGE_RESET             5

`define NUM_STAGES      5
`define STAGE_WIDTH     [2:0]

`define RISCV_LUI           7'b0110111
`define RISCV_AUIPC         7'b0010111
`define RISCV_JAL           7'b1101111
`define RISCV_JALR          7'b1100111
`define RISCV_OP            7'b0110011
`define RISCV_OP_IMM        7'b0010011
`define RISCV_LOAD          7'b0000011
`define RISCV_STORE         7'b0100011
`define RISCV_BRANCH        7'b1100011
`define RISCV_MEM_MISC      7'b0001111
`define RISCV_SYSTEM        7'b1110011

`define RISCV_FENCE         3'b000
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

`define BRANCH_EQ   3'b000
`define BRANCH_NE   3'b001
`define BRANCH_LT   3'b100
`define BRANCH_GE   3'b101
`define BRANCH_LTU  3'b110
`define BRANCH_GEU  3'b111

`define RISCV_STD_OP    7'b0000000
`define RISCV_ALT_OP    7'b0100000

`define RAM_SIZE    8192
`define RAM_START   32'h00001000
`define ROM_SIZE    2048
`define ROM_START   32'h00000000
