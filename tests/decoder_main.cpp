#include <bitset>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Vdecoder.h"
#include "general_test.h"
#include "riscv.h"
#include "verilated.h"

using namespace std;

class TestDecodeRiscvRtype : public GeneralTest<Vdecoder> {
public:
  virtual void test() {
    top->instruction = create_ADD(Register::E::x1, Register::E::x21, Register::E::x17);

    step();

    ASSERT_EQUALS(top->opcode, Opcode::E::OP);
    ASSERT_EQUALS(top->reg_dest, Register::E::x17);
    ASSERT_EQUALS(top->reg_source_0, Register::E::x1);
    ASSERT_EQUALS(top->reg_source_1, Register::E::x21);
    ASSERT_EQUALS(top->imm, 0x00000000);
    ASSERT_EQUALS(top->instr_valid, 0b1);
  }
};

class TestDecodeRiscvUtype : public GeneralTest<Vdecoder> {
public:
  virtual void test() {
    top->instruction = create_LUI(0x00001, Register::E::x17);

    step();

    ASSERT_EQUALS(top->opcode, Opcode::E::LUI);
    ASSERT_EQUALS(top->reg_dest, Register::E::x17);
    ASSERT_EQUALS(top->imm, 0x00001000);
    ASSERT_EQUALS(top->instr_valid, 0b1);
  }
};

class TestDecodeRiscvStype : public GeneralTest<Vdecoder> {
public:
  virtual void test() {
    top->instruction = create_SW(0x00001, 1, 0);

    step();

    ASSERT_EQUALS(top->opcode, Opcode::E::STORE);
    ASSERT_EQUALS(top->imm, 0x00000001);
    ASSERT_EQUALS(top->reg_source_0, 1);
    ASSERT_EQUALS(top->reg_source_1, 0);
    ASSERT_EQUALS(top->instr_valid, 0b1);
  }
};

class TestDecodeRiscvBtype : public GeneralTest<Vdecoder> {
public:
  void do_test_for(int32_t offset, Register::E rs1, Register::E rs2) {
    top->instruction = create_BEQ(offset, rs1, rs2);

    step();

    ASSERT_EQUALS(top->opcode, Opcode::E::BRANCH);
    ASSERT_EQUALS(top->func3, 0b000);
    ASSERT_EQUALS(top->reg_source_0, rs1);
    ASSERT_EQUALS(top->reg_source_1, rs2);
    ASSERT_EQUALS(top->imm, offset);
    ASSERT_EQUALS(top->instr_valid, 0b1);
  }

  virtual void test() {
    for (int r1 = 0; r1 < 32; r1++) {
      for (int r2 = 0; r2 < 32; r2++) {
        do_test_for(-4096, (Register::E)r1, (Register::E)r2);
        do_test_for(-8, (Register::E)r1, (Register::E)r2);
        do_test_for(-4, (Register::E)r1, (Register::E)r2);
        do_test_for(0, (Register::E)r1, (Register::E)r2);
        do_test_for(4, (Register::E)r1, (Register::E)r2);
        do_test_for(8, (Register::E)r1, (Register::E)r2);
        do_test_for(4094, (Register::E)r1, (Register::E)r2);
      }
    }
  }
};

class TestDecodeRiscvItype : public GeneralTest<Vdecoder> {
public:
  virtual void test() {
    top->instruction = create_ADDI(1, Register::E::x17, Register::E::x14);

    step();

    ASSERT_EQUALS(top->opcode, Opcode::E::OP_IMM);
    ASSERT_EQUALS(top->reg_source_0, Register::E::x17);
    ASSERT_EQUALS(top->reg_dest, Register::E::x14);
    ASSERT_EQUALS(top->imm, 1);
    ASSERT_EQUALS(top->func3, 0);
    ASSERT_EQUALS(top->instr_valid, 0b1);

    top->instruction = create_ADDI(2, Register::E::x21, Register::E::x15);

    step();

    ASSERT_EQUALS(top->opcode, Opcode::E::OP_IMM);
    ASSERT_EQUALS(top->reg_source_0, Register::E::x21);
    ASSERT_EQUALS(top->reg_dest, Register::E::x15);
    ASSERT_EQUALS(top->imm, 2);
    ASSERT_EQUALS(top->func3, 0);
    ASSERT_EQUALS(top->instr_valid, 0b1);
  }
};

class TestDecodeRiscvJtype : public GeneralTest<Vdecoder> {
public:
  virtual void test() {
    top->instruction = create_JAL(4, 31);

    step();

    ASSERT_EQUALS(top->opcode, Opcode::E::JAL);
    ASSERT_EQUALS(top->reg_dest, 31);
    ASSERT_EQUALS(top->imm, 4);
    ASSERT_EQUALS(top->instr_valid, 0b1);

    top->instruction = create_JAL(-4, 31);

    step();

    ASSERT_EQUALS(top->opcode, Opcode::E::JAL);
    ASSERT_EQUALS(top->reg_dest, 31);
    ASSERT_EQUALS(top->imm, -4);
    ASSERT_EQUALS(top->instr_valid, 0b1);
  }
};

class TestDecodeFence : public GeneralTest<Vdecoder> {
public:
  virtual void test() {
    top->instruction = create_FENCE(1, 2, 3, Register::E::x1, Register::E::x2);

    step();

    ASSERT_EQUALS(top->instr_valid, 0b1);
  }
};

class TestDecodeEcall : public GeneralTest<Vdecoder> {
public:
  virtual void test() {
    top->instruction = create_ECALL();

    step();

    ASSERT_EQUALS(top->instr_valid, 0b1);
  }
};

class TestDecodeEbreak : public GeneralTest<Vdecoder> {
public:
  virtual void test() {
    top->instruction = create_EBREAK();

    step();

    ASSERT_EQUALS(top->instr_valid, 0b1);
  }
};

class TestDecodeRiscvIllegalOps : public GeneralTest<Vdecoder> {
public:
  virtual void test() {
    top->instruction = 0x00000000;

    step();

    ASSERT_EQUALS(top->instr_valid, 0b0);
  }
};

int main(int argc, char **argv) {
  cout << "---- Testing Decoder" << endl;
  (new TestDecodeRiscvItype())->run("vcds/decode_itype.vcd");
  (new TestDecodeRiscvStype())->run("vcds/decode_stype.vcd");
  (new TestDecodeRiscvBtype())->run("vcds/decode_btype.vcd");
  (new TestDecodeRiscvUtype())->run("vcds/decode_utype.vcd");
  (new TestDecodeRiscvJtype())->run("vcds/decode_jtype.vcd");
  (new TestDecodeRiscvRtype())->run("vcds/decode_rtype.vcd");
  (new TestDecodeFence())->run("vcds/decode_fence.vcd");
  (new TestDecodeEcall())->run("vcds/decode_ecall.vcd");
  (new TestDecodeEbreak())->run("vcds/decode_ebreak.vcd");
  (new TestDecodeRiscvIllegalOps())->run("vcds/decode_illegalops.vcd");
  cout << "$$$$ Decoder passes tests" << endl;
  HANDLE_ERROR_COUNTER;
}
