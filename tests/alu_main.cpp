#include <iostream>
#include <ctime>
#include <cstdlib>
#include <bitset>

#include "Valu.h"
#include "verilated.h"
#include "general_test.h"
#include "riscv.h"

using namespace std;

class TestAluADDI: public GeneralTest<Valu> {
  public:
    virtual void test() {
      step();

      top->opcode = Opcode::E::OP_IMM;
      top->func3 = FUNC3_ADD;
      top->rs1 = 23;
      top->rs2 = 42;

      step();

      ASSERT_EQUALS(top->out, 65);

      top->rs2 = -42;

      step();

      ASSERT_EQUALS(top->out, -19);

    }
};

class TestAluADD: public GeneralTest<Valu> {
  public:
    virtual void test() {
      step();

      top->opcode = Opcode::E::OP;
      top->func3 = FUNC3_ADD;
      top->rs1 = 23;
      top->rs2 = 42;

      step();

      ASSERT_EQUALS(top->out, 65);

      top->rs2 = -42;

      step();

      ASSERT_EQUALS(top->out, -19);

    }
};

class TestAluXORI: public GeneralTest<Valu> {
  public:
    virtual void test() {
      step();

      top->opcode = Opcode::E::OP_IMM;
      top->func3 = FUNC3_XOR;
      top->rs1 = 0b1100;
      top->rs2 = 0b1010;

      step();

      ASSERT_EQUALS(top->out, 0b0110);
    }
};

class TestAluXOR: public GeneralTest<Valu> {
  public:
    virtual void test() {
      step();

      top->opcode = Opcode::E::OP;
      top->func3 = FUNC3_XOR;
      top->rs1 = 0b1100;
      top->rs2 = 0b1010;

      step();

      ASSERT_EQUALS(top->out, 0b0110);
    }
};

class TestAluORI: public GeneralTest<Valu> {
  public:
    virtual void test() {
      step();

      top->opcode = Opcode::E::OP_IMM;
      top->func3 = FUNC3_OR;
      top->rs1 = 0b1100;
      top->rs2 = 0b1010;

      step();

      ASSERT_EQUALS(top->out, 0b1110);
    }
};

class TestAluOR: public GeneralTest<Valu> {
  public:
    virtual void test() {
      step();

      top->opcode = Opcode::E::OP;
      top->func3 = FUNC3_OR;
      top->rs1 = 0b1100;
      top->rs2 = 0b1010;

      step();

      ASSERT_EQUALS(top->out, 0b1110);
    }
};

class TestAluANDI: public GeneralTest<Valu> {
  public:
    virtual void test() {
      step();

      top->opcode = Opcode::E::OP_IMM;
      top->func3 = FUNC3_AND;
      top->rs1 = 0b1100;
      top->rs2 = 0b1010;

      step();

      ASSERT_EQUALS(top->out, 0b1000);
    }
};

class TestAluAND: public GeneralTest<Valu> {
  public:
    virtual void test() {
      step();

      top->opcode = Opcode::E::OP;
      top->func3 = FUNC3_AND;
      top->rs1 = 0b1100;
      top->rs2 = 0b1010;

      step();

      ASSERT_EQUALS(top->out, 0b1000);
    }
};

class TestAluSRLI: public GeneralTest<Valu> {
  public:
    virtual void test() {
      step();

      top->opcode = Opcode::E::OP_IMM;
      top->func3 = FUNC3_SRL;
      top->func7 = FUNC7_STD_OP;
      top->rs1 = 0b10000000000000000000000000000010;
      top->rs2 = 0b00000000000000000000000000000001;

      step();

      ASSERT_EQUALS(top->out, 0b01000000000000000000000000000001);

      top->rs1 = 0b00000000000000000000000000010010;
      top->rs2 = 0b00000000000000000000000000000010;

      step();

      ASSERT_EQUALS(top->out, 0b00000000000000000000000000000100);
    }
};

class TestAluSRL: public GeneralTest<Valu> {
  public:
    virtual void test() {
      step();

      top->opcode = Opcode::E::OP_IMM;
      top->func3 = FUNC3_SRL;
      top->func7 = FUNC7_STD_OP;
      top->rs1 = 0b10000000000000000000000000000010;
      top->rs2 = 0b00000000000000000000000000000001;

      step();

      ASSERT_EQUALS(top->out, 0b01000000000000000000000000000001);

      top->rs1 = 0b00000000000000000000000000010010;
      top->rs2 = 0b00000000000000000000000000000010;

      step();

      ASSERT_EQUALS(top->out, 0b00000000000000000000000000000100);
    }
};

class TestAluSRAI: public GeneralTest<Valu> {
  public:
    virtual void test() {
      step();

      top->opcode = Opcode::E::OP_IMM;
      top->func3 = FUNC3_SRL;
      top->func7 = FUNC7_ALT_OP;
      top->rs1 = 0b10000000000000000000000000000010;
      top->rs2 = 0b00000000000000000000000000000001;

      step();

      ASSERT_EQUALS(top->out, 0b11000000000000000000000000000001);

      top->rs1 = 0b00000000000000000000000000010010;
      top->rs2 = 0b00000000000000000000000000000010;

      step();

      ASSERT_EQUALS(top->out, 0b00000000000000000000000000000100);
    }
};

class TestAluSRA: public GeneralTest<Valu> {
  public:
    virtual void test() {
      step();

      top->opcode = Opcode::E::OP_IMM;
      top->func3 = FUNC3_SRL;
      top->func7 = FUNC7_ALT_OP;
      top->rs1 = 0b10000000000000000000000000000010;
      top->rs2 = 0b00000000000000000000000000000001;

      step();

      ASSERT_EQUALS(top->out, 0b11000000000000000000000000000001);

      top->rs1 = 0b00000000000000000000000000010010;
      top->rs2 = 0b00000000000000000000000000000010;

      step();

      ASSERT_EQUALS(top->out, 0b00000000000000000000000000000100);
    }
};

class TestAluSLTI: public GeneralTest<Valu> {
  public:
    virtual void test() {
      step();

      top->opcode = Opcode::E::OP_IMM;
      top->func3 = FUNC3_SLT;
      top->rs1 = 0x23;
      top->rs2 = 0x34;

      step();

      ASSERT_EQUALS(top->out, 1);

      top->rs1 = 0x23;
      top->rs2 = 0x23;

      step();

      ASSERT_EQUALS(top->out, 0);

      top->rs1 = 0x24;
      top->rs2 = 0x23;

      step();

      ASSERT_EQUALS(top->out, 0);

      top->rs1 = -0x23;
      top->rs2 = 0x23;

      step();

      ASSERT_EQUALS(top->out, 1);
    }
};

class TestAluSLTIU: public GeneralTest<Valu> {
  public:
    virtual void test() {
      step();

      top->opcode = Opcode::E::OP_IMM;
      top->func3 = FUNC3_SLTU;
      top->rs1 = 0x23;
      top->rs2 = 0x34;

      step();

      ASSERT_EQUALS(top->out, 1);

      top->rs1 = 0x23;
      top->rs2 = 0x23;

      step();

      ASSERT_EQUALS(top->out, 0);

      top->rs1 = 0x24;
      top->rs2 = 0x23;

      step();

      ASSERT_EQUALS(top->out, 0);

      top->rs1 = -0x23;
      top->rs2 = 0x23;

      step();

      ASSERT_EQUALS(top->out, 0);
    }
};

class TestAluSLT: public GeneralTest<Valu> {
  public:
    virtual void test() {
      step();

      top->opcode = Opcode::E::OP;
      top->func3 = FUNC3_SLT;
      top->rs1 = 0x23;
      top->rs2 = 0x34;

      step();

      ASSERT_EQUALS(top->out, 1);

      top->rs1 = 0x23;
      top->rs2 = 0x23;

      step();

      ASSERT_EQUALS(top->out, 0);

      top->rs1 = 0x24;
      top->rs2 = 0x23;

      step();

      ASSERT_EQUALS(top->out, 0);

      top->rs1 = -0x23;
      top->rs2 = 0x23;

      step();

      ASSERT_EQUALS(top->out, 1);
    }
};

class TestAluSLTU: public GeneralTest<Valu> {
  public:
    virtual void test() {
      step();

      top->opcode = Opcode::E::OP;
      top->func3 = FUNC3_SLTU;
      top->rs1 = 0x23;
      top->rs2 = 0x34;

      step();

      ASSERT_EQUALS(top->out, 1);

      top->rs1 = 0x23;
      top->rs2 = 0x23;

      step();

      ASSERT_EQUALS(top->out, 0);

      top->rs1 = 0x24;
      top->rs2 = 0x23;

      step();

      ASSERT_EQUALS(top->out, 0);

      top->rs1 = -0x23;
      top->rs2 = 0x23;

      step();

      ASSERT_EQUALS(top->out, 0);
    }
};

class TestAluSUB: public GeneralTest<Valu> {
  public:
    virtual void test() {
      step();

      top->opcode = Opcode::E::OP;
      top->func3 = FUNC3_ADD;
      top->func7 = FUNC7_ALT_OP;
      top->rs1 = 23;
      top->rs2 = 42;

      step();

      ASSERT_EQUALS(top->out, -19);

      top->rs2 = -42;

      step();

      ASSERT_EQUALS(top->out, 65);

    }
};

int main(int argc, char** argv) {
  cout << "---- ALU tests" << endl;
  (new TestAluADDI())->run("vcds/alu_addi.vcd");
  (new TestAluADD())->run("vcds/alu_add.vcd");
  (new TestAluXORI())->run("vcds/alu_xori.vcd");
  (new TestAluXOR())->run("vcds/alu_xor.vcd");
  (new TestAluORI())->run("vcds/alu_ori.vcd");
  (new TestAluOR())->run("vcds/alu_or.vcd");
  (new TestAluANDI())->run("vcds/alu_andi.vcd");
  (new TestAluAND())->run("vcds/alu_and.vcd");
  (new TestAluSRLI())->run("vcds/alu_srli.vcd");
  (new TestAluSRL())->run("vcds/alu_srl.vcd");
  (new TestAluSRAI())->run("vcds/alu_srai.vcd");
  (new TestAluSRA())->run("vcds/alu_sra.vcd");
  (new TestAluSLTI())->run("vcds/alu_slti.vcd");
  (new TestAluSLTIU())->run("vcds/alu_sltiu.vcd");
  (new TestAluSLT())->run("vcds/alu_slt.vcd");
  (new TestAluSLTU())->run("vcds/alu_sltu.vcd");
  (new TestAluSUB())->run("vcds/alu_sub.vcd");
  cout << "$$$$ ALU tests pass" << endl;
  HANDLE_ERROR_COUNTER;
}
