#pragma once

#include <bitset>
#include <cstdint>
#include <iostream>

const int STAGE_FETCH = 0;
const int STAGE_DECODE = 1;
const int STAGE_EXECUTE = 2;
const int STAGE_MEMORY = 3;
const int STAGE_REGISTER_UPDATE = 4;
const int STAGE_COUNT = 5;

struct Register {
   enum E {
      x0   = 0,
      zero = 0,
      x1   = 1,
      ra   = 1,
      x2   = 2,
      sp   = 2,
      x3   = 3,
      gp   = 3,
      x4   = 4,
      tp   = 4,
      x5   = 5,
      t0   = 5,
      x6   = 6,
      t1   = 6,
      x7   = 7,
      t2   = 7,
      x8   = 8,
      s0   = 8,
      fp   = 8,
      x9   = 9,
      s1   = 9,
      x10  = 10,
      a0   = 10,
      x11  = 11,
      a1   = 11,
      x12  = 12,
      a2   = 12,
      x13  = 13,
      a3   = 13,
      x14  = 14,
      a4   = 14,
      x15  = 15,
      a5   = 15,
      x16  = 16,
      a6   = 16,
      x17  = 17,
      a7   = 17,
      x18  = 18,
      s2   = 18,
      x19  = 19,
      s3   = 19,
      x20  = 20,
      s4   = 20,
      x21  = 21,
      s5   = 21,
      x22  = 22,
      s6   = 22,
      x23  = 23,
      s7   = 23,
      x24  = 24,
      s8   = 24,
      x25  = 25,
      s9   = 25,
      x26  = 26,
      s10  = 26,
      x27  = 27,
      s11  = 27,
      x28  = 28,
      t3   = 28,
      x29  = 29,
      t4   = 29,
      x30  = 30,
      t5   = 30,
      x31  = 31,
      t6   = 31,
   };
};

struct Opcode {
    enum E {
        AUIPC    = 0b0010111,
        LUI      = 0b0110111,
        JAL      = 0b1101111,
        JALR     = 0b1100111,
        BRANCH   = 0b1100011,
        LOAD     = 0b0000011,
        STORE    = 0b0100011,
        OP_IMM   = 0b0010011,
        OP       = 0b0110011,
        MEM_MISC = 0b0001111,
        SYSTEM   = 0b1110011
    };
};

struct SystemFunc3 {
    enum E {
        CSRRW  = 0b001,
        CSRRWI = 0b101,
        CSRRS  = 0b010,
        CSRRSI = 0b110,
        CSRRC  = 0b011,
        CSRRCI = 0b111,
    };
};

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

struct CSR {
    enum E {
        MVENDORID = 0xF11,
        MARCHID = 0xF12,
        MIMPID = 0xF13,
        MHARTID = 0xF14,
        MISA = 0x301,
        MSCRATCH = 0x340,
    };
};

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
  tmp = tmp | Opcode::E::LUI;
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
  tmp |= Opcode::E::BRANCH;
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
  tmp |= Opcode::E::BRANCH;
  tmp |= (FUNC3_BNE << 12);
  return tmp;
}

uint32_t create_ADDI(int32_t immediate, Register::E rs1, Register::E rd) {
  int32_t tmp = (0b111111111111 & immediate) << 20;
  tmp = tmp | (0b11111 & rs1) << 15;
  tmp = tmp | (0b11111 & rd) << 7;
  tmp = tmp | Opcode::E::OP_IMM;
  return tmp;
}

uint32_t create_SRLI(uint32_t immediate, Register::E rs1, Register::E rd) {
  int32_t tmp = 0;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= (0b11111 & immediate) << 20;
  tmp |= (0b11111 & rd) << 7;
  tmp |= Opcode::E::OP_IMM;
  tmp |= (FUNC3_SRL << 12);
  tmp |= (FUNC7_STD_OP << 25);
  return tmp;
}

uint32_t create_SLLI(uint32_t immediate, Register::E rs1, Register::E rd) {
  int32_t tmp = 0;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= (0b11111 & immediate) << 20;
  tmp |= (0b11111 & rd) << 7;
  tmp |= Opcode::E::OP_IMM;
  tmp |= (FUNC3_SLL << 12);
  tmp |= (FUNC7_STD_OP << 25);
  return tmp;
}

uint32_t create_SLTI(uint32_t immediate, Register::E rs1, Register::E rd) {
  int32_t tmp = 0;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= (0b11111 & immediate) << 20;
  tmp |= (0b11111 & rd) << 7;
  tmp |= Opcode::E::OP_IMM;
  tmp |= (FUNC3_SLT << 12);
  tmp |= (FUNC7_STD_OP << 25);
  return tmp;
}

uint32_t create_SLTIU(uint32_t immediate, Register::E rs1, Register::E rd) {
  int32_t tmp = 0;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= (0b11111 & immediate) << 20;
  tmp |= (0b11111 & rd) << 7;
  tmp |= Opcode::E::OP_IMM;
  tmp |= (FUNC3_SLTU << 12);
  tmp |= (FUNC7_STD_OP << 25);
  return tmp;
}

uint32_t create_SLT(Register::E rs1, Register::E rs2, Register::E rd) {
  int32_t tmp = 0;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= (0b11111 & rs2) << 20;
  tmp |= (0b11111 & rd) << 7;
  tmp |= Opcode::E::OP_IMM;
  tmp |= (FUNC3_SLT << 12);
  tmp |= (FUNC7_STD_OP << 25);
  return tmp;
}

uint32_t create_SLTU(Register::E rs1, Register::E rs2, Register::E rd) {
  int32_t tmp = 0;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= (0b11111 & rs2) << 20;
  tmp |= (0b11111 & rd) << 7;
  tmp |= Opcode::E::OP_IMM;
  tmp |= (FUNC3_SLTU << 12);
  tmp |= (FUNC7_STD_OP << 25);
  return tmp;
}

uint32_t create_SRAI(uint32_t immediate, Register::E rs1, Register::E rd) {
  int32_t tmp = 0;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= (0b11111 & immediate) << 20;
  tmp |= (0b11111 & rd) << 7;
  tmp |= Opcode::E::OP_IMM;
  tmp |= (FUNC3_SRL << 12);
  tmp |= (FUNC7_ALT_OP << 25);
  return tmp;
}

uint32_t create_AUIPC(int32_t immediate, Register::E rd) {
  int32_t tmp = (0b11111111111111111111 & immediate) << 12;
  tmp = tmp | (0b11111 & rd) << 7;
  tmp = tmp | Opcode::E::AUIPC;
  return tmp;
}

uint32_t create_ADD(Register::E rs1, Register::E rs2, Register::E rd) {
  int32_t tmp = 0;
  tmp = tmp | (0b11111 & rs1) << 15;
  tmp = tmp | (0b11111 & rs2) << 20;
  tmp = tmp | (0b11111 & rd) << 7;
  tmp = tmp | Opcode::E::OP;
  return tmp;
}

uint32_t create_SUB(Register::E rs1, Register::E rs2, Register::E rd) {
  int32_t tmp = 0;
  tmp = tmp | (0b11111 & rs1) << 15;
  tmp = tmp | (0b11111 & rs2) << 20;
  tmp = tmp | (0b11111 & rd) << 7;
  tmp = tmp | Opcode::E::OP;
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
  tmp |= Opcode::E::LOAD;
  return tmp;
}

uint32_t create_LBU(int32_t offset, Register::E rs1, Register::E rd) {
  int32_t tmp = 0;
  tmp |= (0b111111111111 & offset) << 20;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= FUNC3_LBU << 12;
  tmp |= (0b11111 & rd) << 7;
  tmp |= Opcode::E::LOAD;
  return tmp;
}

uint32_t create_LH(int32_t offset, Register::E rs1, Register::E rd) {
  int32_t tmp = 0;
  tmp |= (0b111111111111 & offset) << 20;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= FUNC3_LH << 12;
  tmp |= (0b11111 & rd) << 7;
  tmp |= Opcode::E::LOAD;
  return tmp;
}

uint32_t create_LHU(int32_t offset, Register::E rs1, Register::E rd) {
  int32_t tmp = 0;
  tmp |= (0b111111111111 & offset) << 20;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= FUNC3_LHU << 12;
  tmp |= (0b11111 & rd) << 7;
  tmp |= Opcode::E::LOAD;
  return tmp;
}

uint32_t create_LW(int32_t offset, Register::E rs1, Register::E rd) {
  int32_t tmp = 0;
  tmp |= (0b111111111111 & offset) << 20;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= FUNC3_LW << 12;
  tmp |= (0b11111 & rd) << 7;
  tmp |= Opcode::E::LOAD;
  return tmp;
}

uint32_t create_SB(int32_t offset, Register::E rs1, Register::E rs2) {
  int32_t tmp = 0;
  tmp |= (0b111111100000 & offset) << 25;
  tmp |= (0b11111 & offset) << 7;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= FUNC3_SB << 12;
  tmp |= (0b11111 & rs2) << 20;
  tmp |= Opcode::E::STORE;
  return tmp;
}

uint32_t create_SH(int32_t offset, Register::E rs1, Register::E rs2) {
  int32_t tmp = 0;
  tmp |= (0b111111100000 & offset) << 25;
  tmp |= (0b11111 & offset) << 7;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= FUNC3_SH << 12;
  tmp |= (0b11111 & rs2) << 20;
  tmp |= Opcode::E::STORE;
  return tmp;
}

uint32_t create_SW(int32_t offset, Register::E rs1, Register::E rs2) {
  int32_t tmp = 0;
  tmp |= (0b111111100000 & offset) << 25;
  tmp |= (0b11111 & offset) << 7;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= FUNC3_SW << 12;
  tmp |= (0b11111 & rs2) << 20;
  tmp |= Opcode::E::STORE;
  return tmp;
}

uint32_t create_JALR(int32_t offset, Register::E rs1, Register::E rd) {
  int32_t tmp = 0;
  tmp |= (0b111111111111 & offset) << 20;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= (0b11111 & rd) << 7;
  tmp |= Opcode::E::JALR;
  return tmp;
}

uint32_t create_JAL(int32_t offset, Register::E rd) {
  int32_t tmp = 0;
  tmp |= (0b111111111111 & offset) << 20;
  tmp |= (0b11111 & rd) << 7;
  tmp |= Opcode::E::JAL;
  return tmp;
}

uint32_t create_FENCE(uint8_t fm, uint8_t pred, uint32_t succ,  Register::E rs1, Register::E rd) {
  int32_t tmp = 0;
  tmp |= (0b1111 & fm) << 28;
  tmp |= (0b1111 & pred) << 24;
  tmp |= (0b1111 & succ) << 20;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= (0b11111 & rd) << 7;
  tmp |= Opcode::E::MEM_MISC;
  return tmp;
}

uint32_t create_ECALL() {
  int32_t tmp = 0;
  tmp |= Opcode::E::SYSTEM;
  return tmp;
}

uint32_t create_EBREAK() {
  int32_t tmp = 0;
  tmp |= 1 << 20;
  tmp |= Opcode::E::SYSTEM;
  return tmp;
}

uint32_t create_CSRRW(uint32_t csr, Register::E rs1, Register::E rd) {
  int32_t tmp = 0;
  tmp |= (0b111111111111 & csr) << 20;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= SystemFunc3::E::CSRRW << 12;
  tmp |= (0b11111 & rd) << 7;
  tmp |= Opcode::E::SYSTEM;
  return tmp;
}

uint32_t create_CSRRS(uint32_t csr, Register::E rs1, Register::E rd) {
  int32_t tmp = 0;
  tmp |= (0b111111111111 & csr) << 20;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= SystemFunc3::E::CSRRS << 12;
  tmp |= (0b11111 & rd) << 7;
  tmp |= Opcode::E::SYSTEM;
  return tmp;
}

uint32_t create_CSRRC(CSR::E csr, Register::E rs1, Register::E rd) {
  int32_t tmp = 0;
  tmp |= (0b111111111111 & csr) << 20;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= SystemFunc3::E::CSRRC << 12;
  tmp |= (0b11111 & rd) << 7;
  tmp |= Opcode::E::SYSTEM;
  return tmp;
}

uint32_t create_CSRRWI(uint32_t csr, Register::E rs1, Register::E rd) {
  int32_t tmp = 0;
  tmp |= (0b111111111111 & csr) << 20;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= SystemFunc3::E::CSRRWI << 12;
  tmp |= (0b11111 & rd) << 7;
  tmp |= Opcode::E::SYSTEM;
  return tmp;
}

uint32_t create_CSRRSI(uint32_t csr, Register::E rs1, Register::E rd) {
  int32_t tmp = 0;
  tmp |= (0b111111111111 & csr) << 20;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= SystemFunc3::E::CSRRSI << 12;
  tmp |= (0b11111 & rd) << 7;
  tmp |= Opcode::E::SYSTEM;
  return tmp;
}

uint32_t create_CSRRCI(uint32_t csr, Register::E rs1, Register::E rd) {
  int32_t tmp = 0;
  tmp |= (0b111111111111 & csr) << 20;
  tmp |= (0b11111 & rs1) << 15;
  tmp |= SystemFunc3::E::CSRRCI << 12;
  tmp |= (0b11111 & rd) << 7;
  tmp |= Opcode::E::SYSTEM;
  return tmp;
}
