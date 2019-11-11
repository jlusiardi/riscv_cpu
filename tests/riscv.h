#pragma once

#include <bitset>
#include <cstdint>

const int STAGE_FETCH = 0;
const int STAGE_DECODE = 1;
const int STAGE_EXECUTE = 2;
const int STAGE_MEMORY = 3;
const int STAGE_REGISTER_UPDATE = 4;
const int STAGE_COUNT = 5;

struct Register {
   enum E {
      x0 = 0,
      x1,
      x2,
      x3,
      x4,
      x5,
      x6,
      x7,
      x8,
      x9,
      x10,
      x11,
      x12,
      x13,
      x14,
      x15,
      x16,
      x17,
      x18,
      x19,
      x20,
      x21,
      x22,
      x23,
      x24,
      x25,
      x26,
      x27,
      x28,
      x29,
      x30,
      x31
   };
};

const int OPCODE_AUIPC         = 0b0010111;
const int OPCODE_ALU_IMMEDIATE = 0b0010011;
const int OPCODE_ALU_REG       = 0b0110011;
const int OPCODE_STORE         = 0b0100011;
const int OPCODE_LOAD          = 0b0000011;
const int OPCODE_BRANCH        = 0b1100011;
const int OPCODE_JALR          = 0b1100111;
const int OPCODE_JAL           = 0b1101111;

const int FUNC3_ADD = 0b000;
const int FUNC3_XOR = 0b100;
const int FUNC3_OR  = 0b110;
const int FUNC3_AND = 0b111;
const int FUNC3_SRL = 0b101;
const int FUNC3_SLL = 0b001;
const int FUNC3_SLT = 0b010;
const int FUNC3_SLTU= 0b011;
const int FUNC3_SB  = 0b000;
const int FUNC3_LB  = 0b000;
const int FUNC3_LBU = 0b100;
const int FUNC3_SH  = 0b001;
const int FUNC3_LH  = 0b001;
const int FUNC3_LHU = 0b101;
const int FUNC3_SW  = 0b010;
const int FUNC3_LW  = 0b010;
const int FUNC3_BEQ = 0b000;
const int FUNC3_BNE = 0b001;
const int FUNC3_BLT = 0b100;
const int FUNC3_BGE = 0b101;
const int FUNC3_BLTU = 0b110;
const int FUNC3_BGEU = 0b111;

const int FUNC7_STD_OP = 0b0000000;
const int FUNC7_ALT_OP = 0b0100000;

/**
 * Macro to insert a 32bit value (mostly a risc-v instruction) into a target
 * consisting of 8bit slots (like ROM or RAM).
 */
#define insert_4bytes(TARGET, POSITION, COMMAND) {\
        uint32_t instr = (COMMAND); \
        (TARGET)[(POSITION) + 0] = (instr >> 0) & 0xFF; \
        (TARGET)[(POSITION) + 1] = (instr >> 8) & 0xFF; \
        (TARGET)[(POSITION) + 2] = (instr >> 16) & 0xFF; \
        (TARGET)[(POSITION) + 3] = (instr >> 24) & 0xFF; \
    }

uint32_t create_LUI(int32_t immediate, Register::E rd) {
  int32_t tmp = (0xFFFFF & immediate) << 12;
  tmp = tmp | (0b11111 & rd) << 7;
  tmp = tmp | 0b0110111;
  return tmp;
}

uint32_t create_SW(int32_t offset, uint8_t rs1, uint8_t rs2) {
  int p1 = (0b11111 & offset) << 7;
  int p2 = (0b11111111111100000 & offset) << 20;
  int32_t tmp = p1 | p2;
  tmp = tmp | (0b11111 & rs1) << 15;
  tmp = tmp | (0b11111 & rs2) << 20;
  tmp = tmp | 0b0100011 | (0b010 << 12);
  return tmp;
}

uint32_t create_BEQ(int32_t offset, Register::E rs1, Register::E rs2) {
  std::bitset<12> bs = std::bitset<12>((offset / 2));
  std::bitset<5> imm1;
  std::bitset<7> imm2;

  imm1[0] = bs[10];
  for (int i = 1; i < 5; i++) {
    imm1[i] = bs[i - 1];
  }

  for (int i = 0; i < 6; i++) {
    imm2[i] = bs[i + 4];
  }
  imm2[6] = bs[11];

  int32_t tmp = (imm2.to_ulong() << 25) | (imm1.to_ulong() << 7);
  tmp |= (0b11111 & rs1) << 15;
  tmp |= (0b11111 & rs2) << 20;
  tmp |= OPCODE_BRANCH;
  tmp |= (FUNC3_BEQ << 12);
  return tmp;
}

uint32_t create_BNE(int32_t offset, Register::E rs1, Register::E rs2) {
  std::bitset<12> bs = std::bitset<12>((offset / 2));
  std::bitset<5> imm1;
  std::bitset<7> imm2;

  imm1[0] = bs[10];
  for (int i = 1; i < 5; i++) {
    imm1[i] = bs[i - 1];
  }

  for (int i = 0; i < 6; i++) {
    imm2[i] = bs[i + 4];
  }
  imm2[6] = bs[11];

  int32_t tmp = (imm2.to_ulong() << 25) | (imm1.to_ulong() << 7);
  tmp |= (0b11111 & rs1) << 15;
  tmp |= (0b11111 & rs2) << 20;
  tmp |= OPCODE_BRANCH;
  tmp |= (FUNC3_BNE << 12);
  return tmp;
}

uint32_t create_ADDI(int32_t immediate, Register::E rs1, Register::E rd) {
  int32_t tmp = (0b111111111111 & immediate) << 20;
  tmp = tmp | (0b11111 & rs1) << 15;
  tmp = tmp | (0b11111 & rd) << 7;
  tmp = tmp | OPCODE_ALU_IMMEDIATE;
  return tmp;
}

uint32_t create_SRLI(uint32_t immediate, Register::E rs1, Register::E rd) {
  int32_t tmp = 0;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= (0b11111 & immediate) << 20;
  tmp |= (0b11111 & rd) << 7;
  tmp |= OPCODE_ALU_IMMEDIATE;
  tmp |= (FUNC3_SRL << 12);
  tmp |= (FUNC7_STD_OP << 25);
  return tmp;
}

uint32_t create_SLLI(uint32_t immediate, Register::E rs1, Register::E rd) {
  int32_t tmp = 0;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= (0b11111 & immediate) << 20;
  tmp |= (0b11111 & rd) << 7;
  tmp |= OPCODE_ALU_IMMEDIATE;
  tmp |= (FUNC3_SLL << 12);
  tmp |= (FUNC7_STD_OP << 25);
  return tmp;
}

uint32_t create_SLTI(uint32_t immediate, Register::E rs1, Register::E rd) {
  int32_t tmp = 0;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= (0b11111 & immediate) << 20;
  tmp |= (0b11111 & rd) << 7;
  tmp |= OPCODE_ALU_IMMEDIATE;
  tmp |= (FUNC3_SLT << 12);
  tmp |= (FUNC7_STD_OP << 25);
  return tmp;
}

uint32_t create_SLTIU(uint32_t immediate, Register::E rs1, Register::E rd) {
  int32_t tmp = 0;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= (0b11111 & immediate) << 20;
  tmp |= (0b11111 & rd) << 7;
  tmp |= OPCODE_ALU_IMMEDIATE;
  tmp |= (FUNC3_SLTU << 12);
  tmp |= (FUNC7_STD_OP << 25);
  return tmp;
}

uint32_t create_SLT(Register::E rs1, Register::E rs2, Register::E rd) {
  int32_t tmp = 0;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= (0b11111 & rs2) << 20;
  tmp |= (0b11111 & rd) << 7;
  tmp |= OPCODE_ALU_IMMEDIATE;
  tmp |= (FUNC3_SLT << 12);
  tmp |= (FUNC7_STD_OP << 25);
  return tmp;
}

uint32_t create_SLTU(Register::E rs1, Register::E rs2, Register::E rd) {
  int32_t tmp = 0;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= (0b11111 & rs2) << 20;
  tmp |= (0b11111 & rd) << 7;
  tmp |= OPCODE_ALU_IMMEDIATE;
  tmp |= (FUNC3_SLTU << 12);
  tmp |= (FUNC7_STD_OP << 25);
  return tmp;
}

uint32_t create_SRAI(uint32_t immediate, Register::E rs1, Register::E rd) {
  int32_t tmp = 0;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= (0b11111 & immediate) << 20;
  tmp |= (0b11111 & rd) << 7;
  tmp |= OPCODE_ALU_IMMEDIATE;
  tmp |= (FUNC3_SRL << 12);
  tmp |= (FUNC7_ALT_OP << 25);
  return tmp;
}

uint32_t create_AUIPC(int32_t immediate, Register::E rd) {
  int32_t tmp = (0b11111111111111111111 & immediate) << 12;
  tmp = tmp | (0b11111 & rd) << 7;
  tmp = tmp | OPCODE_AUIPC;
  return tmp;
}

uint32_t create_ADD(Register::E rs1, Register::E rs2, Register::E rd) {
  int32_t tmp = 0;
  tmp = tmp | (0b11111 & rs1) << 15;
  tmp = tmp | (0b11111 & rs2) << 20;
  tmp = tmp | (0b11111 & rd) << 7;
  tmp = tmp | OPCODE_ALU_REG;
  return tmp;
}

uint32_t create_SUB(Register::E rs1, Register::E rs2, Register::E rd) {
  int32_t tmp = 0;
  tmp = tmp | (0b11111 & rs1) << 15;
  tmp = tmp | (0b11111 & rs2) << 20;
  tmp = tmp | (0b11111 & rd) << 7;
  tmp = tmp | OPCODE_ALU_REG;
  tmp = tmp | (FUNC7_ALT_OP << 25);
  return tmp;
}

uint32_t create_JAL(int32_t offset, uint8_t rd) {
  std::bitset<20> repr, bs = std::bitset<20>((offset / 2));
  for (int i = 0; i < 10; i++) {
    repr[9 + i] = bs[i];
  }
  repr[8] = bs[10];
  for (int i = 0; i < 10; i++) {
    repr[i] = bs[11 + i];
  }
  if (offset < 0) {
    repr[19] = 1;
  }

  repr[18] = bs[11];

  int32_t tmp = repr.to_ulong() << 12;
  tmp = tmp | (0b11111 & rd) << 7;
  tmp = tmp | 0b1101111;
  return tmp;
}

uint32_t create_LB(int32_t offset, Register::E rs1, Register::E rd) {
  int32_t tmp = 0;
  tmp |= (0b111111111111 & offset) << 20;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= FUNC3_LB << 12;
  tmp |= (0b11111 & rd) << 7;
  tmp |= OPCODE_LOAD;
  return tmp;
}

uint32_t create_LBU(int32_t offset, Register::E rs1, Register::E rd) {
  int32_t tmp = 0;
  tmp |= (0b111111111111 & offset) << 20;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= FUNC3_LBU << 12;
  tmp |= (0b11111 & rd) << 7;
  tmp |= OPCODE_LOAD;
  return tmp;
}

uint32_t create_LH(int32_t offset, Register::E rs1, Register::E rd) {
  int32_t tmp = 0;
  tmp |= (0b111111111111 & offset) << 20;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= FUNC3_LH << 12;
  tmp |= (0b11111 & rd) << 7;
  tmp |= OPCODE_LOAD;
  return tmp;
}

uint32_t create_LHU(int32_t offset, Register::E rs1, Register::E rd) {
  int32_t tmp = 0;
  tmp |= (0b111111111111 & offset) << 20;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= FUNC3_LHU << 12;
  tmp |= (0b11111 & rd) << 7;
  tmp |= OPCODE_LOAD;
  return tmp;
}

uint32_t create_LW(int32_t offset, Register::E rs1, Register::E rd) {
  int32_t tmp = 0;
  tmp |= (0b111111111111 & offset) << 20;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= FUNC3_LW << 12;
  tmp |= (0b11111 & rd) << 7;
  tmp |= OPCODE_LOAD;
  return tmp;
}

uint32_t create_SB(int32_t offset, Register::E rs1, Register::E rs2) {
  int32_t tmp = 0;
  tmp |= (0b111111100000 & offset) << 25;
  tmp |= (0b11111 & offset) << 7;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= FUNC3_SB << 12;
  tmp |= (0b11111 & rs2) << 20;
  tmp |= OPCODE_STORE;
  return tmp;
}

uint32_t create_SH(int32_t offset, Register::E rs1, Register::E rs2) {
  int32_t tmp = 0;
  tmp |= (0b111111100000 & offset) << 25;
  tmp |= (0b11111 & offset) << 7;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= FUNC3_SH << 12;
  tmp |= (0b11111 & rs2) << 20;
  tmp |= OPCODE_STORE;
  return tmp;
}

uint32_t create_SW(int32_t offset, Register::E rs1, Register::E rs2) {
  int32_t tmp = 0;
  tmp |= (0b111111100000 & offset) << 25;
  tmp |= (0b11111 & offset) << 7;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= FUNC3_SW << 12;
  tmp |= (0b11111 & rs2) << 20;
  tmp |= OPCODE_STORE;
  return tmp;
}

uint32_t create_JALR(int32_t offset, Register::E rs1, Register::E rd) {
  int32_t tmp = 0;
  tmp |= (0b111111111111 & offset) << 20;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= (0b11111 & rd) << 7;
  tmp |= OPCODE_JALR;
  return tmp;
}

uint32_t create_JAL(int32_t offset, Register::E rd) {
  int32_t tmp = 0;
  tmp |= (0b111111111111 & offset) << 20;
  tmp |= (0b11111 & rd) << 7;
  tmp |= OPCODE_JAL;
  return tmp;
}
