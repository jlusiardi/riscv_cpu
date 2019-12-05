#include <iostream>
#include <bitset>

#include "verilated.h"
#include "Vcpu.h"
#include "general_test.h"
#include "riscv.h"

using namespace std;

#define ROM cpu__DOT__mem__DOT__rom__DOT__mem
#define RAM cpu__DOT__mem__DOT__ram__DOT__mem
#define PC cpu__DOT__pc_register__DOT__Q_data
#define STAGE cpu__DOT__stage_counter__DOT__data
#define REGISTERS cpu__DOT__register_file__DOT__registers

class TestVcpuAddi: public GeneralTest<Vcpu> {
  public:
    virtual void test() {
      insert_4bytes(top->ROM, 0, create_ADDI(Register::E::x1, Register::E::x0, 1));
      insert_4bytes(top->ROM, 4, create_ADDI(Register::E::x2, Register::E::x0, 2));
      insert_4bytes(top->ROM, 8, create_ADDI(Register::E::x3, Register::E::x1, 3));

      top->rst = 0;
      clock_cycle();
      clock_cycle();
      top->rst = 1;
      EXECUTE_INSTR;
      ASSERT_EQUALS(top->REGISTERS[0], 1);
      EXECUTE_INSTR;
      ASSERT_EQUALS(top->REGISTERS[1], 2);
      EXECUTE_INSTR;
      ASSERT_EQUALS(top->REGISTERS[2], 4);
    }
};

class TestVcpuAdd: public GeneralTest<Vcpu> {
  public:
    virtual void test() {
      insert_4bytes(top->ROM, 0, create_ADDI(Register::E::x1, Register::E::x0, 2));
      insert_4bytes(top->ROM, 4, create_ADD(Register::E::x2, Register::E::x1, Register::E::x1));
      insert_4bytes(top->ROM, 8, create_ADD(Register::E::x3, Register::E::x1, Register::E::x2));

      top->rst = 0;
      clock_cycle();
      clock_cycle();
      top->rst = 1;
      EXECUTE_INSTR;
      ASSERT_EQUALS(top->REGISTERS[0], 2);
      EXECUTE_INSTR;
      ASSERT_EQUALS(top->REGISTERS[1], 4);
      EXECUTE_INSTR;
      ASSERT_EQUALS(top->REGISTERS[2], 6);
    }
};

class TestVcpuSub: public GeneralTest<Vcpu> {
  public:
    virtual void test() {
      insert_4bytes(top->ROM, 0, create_ADDI(Register::E::x1, Register::E::x0, 2));
      insert_4bytes(top->ROM, 4, create_ADDI(Register::E::x2, Register::E::x0, 3));
      insert_4bytes(top->ROM, 8, create_SUB(Register::E::x2, Register::E::x1, Register::E::x3));
      insert_4bytes(top->ROM, 12, create_SUB(Register::E::x1, Register::E::x2, Register::E::x3));

      top->rst = 0;
      clock_cycle();
      clock_cycle();
      top->rst = 1;
      EXECUTE_INSTR;
      ASSERT_EQUALS(top->REGISTERS[0], 2);
      EXECUTE_INSTR;
      ASSERT_EQUALS(top->REGISTERS[1], 3);
      EXECUTE_INSTR;
      ASSERT_EQUALS(top->REGISTERS[2], 1);
      EXECUTE_INSTR;
      ASSERT_EQUALS(top->REGISTERS[2], -1);
    }
};

class TestVcpuLi: public GeneralTest<Vcpu> {
  public:
    virtual void test() {
      insert_4bytes(top->ROM, 0, create_LUI(Register::E::x15, 0xaaaab));
      insert_4bytes(top->ROM, 4, create_ADDI(Register::E::x15, Register::E::x15, -1366));

      top->rst = 0;
      clock_cycle();
      clock_cycle();
      top->rst = 1;
      EXECUTE_INSTR;
      ASSERT_EQUALS(top->REGISTERS[14], 0b10101010101010101011000000000000);
      EXECUTE_INSTR;
      ASSERT_EQUALS(top->REGISTERS[14], 0b10101010101010101010101010101010);
    }
};

class TestVcpuAuipc: public GeneralTest<Vcpu> {
  public:
    virtual void test() {
      insert_4bytes(top->ROM, 0, create_AUIPC(Register::E::x1, 1048575));
      insert_4bytes(top->ROM, 4, create_AUIPC(Register::E::x2, 1048575));
      insert_4bytes(top->ROM, 8, create_AUIPC(Register::E::x3, 1048575));

      top->rst = 0;
      clock_cycle();
      clock_cycle();
      top->rst = 1;
      EXECUTE_INSTR;
      ASSERT_EQUALS(top->REGISTERS[0], 0xfffff000);
      EXECUTE_INSTR;
      ASSERT_EQUALS(top->REGISTERS[1], 0xfffff004);
      EXECUTE_INSTR;
      ASSERT_EQUALS(top->REGISTERS[2], 0xfffff008);
    }
};

class TestVcpuLb_no_signextend: public GeneralTest<Vcpu> {
  public:
    virtual void test() {
      top->REGISTERS[0] = 0x0000;
      top->REGISTERS[1] = 0x1000;
      top->RAM[0] = 0x78;
      insert_4bytes(top->ROM, 0, create_LB(Register::E::x1, 0, Register::E::x2));

      top->rst = 0;
      clock_cycle();
      clock_cycle();
      top->rst = 1;
      EXECUTE_INSTR;

      ASSERT_EQUALS(top->REGISTERS[0], 0x78);
    }
};

class TestVcpuLb_signextend: public GeneralTest<Vcpu> {
  public:
    virtual void test() {
      top->REGISTERS[0] = 0x0000;
      top->REGISTERS[1] = 0x1000;
      top->RAM[0] = 0x88;
      insert_4bytes(top->ROM, 0, create_LB(Register::E::x1, 0, Register::E::x2));

      top->rst = 0;
      clock_cycle();
      clock_cycle();
      top->rst = 1;
      EXECUTE_INSTR;

      ASSERT_EQUALS(top->REGISTERS[0], 0xFFFFFF88);
    }
};

class TestVcpuLbu: public GeneralTest<Vcpu> {
  public:
    virtual void test() {
      insert_4bytes(top->RAM, 3, -1);
      top->RAM[8] = 0x21;
      top->RAM[9] = 0x43;
      top->RAM[10] = 0x65;
      top->RAM[11] = 0x87;

      insert_4bytes(top->ROM, 0, create_LUI(Register::E::x2, 0x00001));
      insert_4bytes(top->ROM, 4, create_LBU(Register::E::x1, 3, Register::E::x2));
      insert_4bytes(top->ROM, 8, create_LBU(Register::E::x1, 8, Register::E::x2));

      top->rst = 0;
      clock_cycle();
      clock_cycle();
      top->rst = 1;
      EXECUTE_INSTR;

      ASSERT_EQUALS(top->REGISTERS[1], 0x1000);
      EXECUTE_INSTR;

      // storing -1 (or 0xffffffff) into 32bit and reading only 8 bit unsigned,
      // stuffing it into 32bit makes this 0xff
      ASSERT_EQUALS(top->REGISTERS[0], 0xff);
      EXECUTE_INSTR;

      ASSERT_EQUALS(top->REGISTERS[0], 0x21);
    }
};

class TestVcpuLh: public GeneralTest<Vcpu> {
  public:
    virtual void test() {
      insert_4bytes(top->RAM, 0, 0x12345678);
      insert_4bytes(top->RAM, 4, 0xFFFFFFFF);
      top->RAM[8] = 0x21;
      top->RAM[9] = 0x43;
      top->RAM[10] = 0x65;
      top->RAM[11] = 0x87;

      insert_4bytes(top->ROM, 0, create_LUI(Register::E::x2, 0x00001));
      insert_4bytes(top->ROM, 4, create_LH(Register::E::x1, 0, Register::E::x2));
      insert_4bytes(top->ROM, 8, create_LH(Register::E::x1, 4, Register::E::x2));
      insert_4bytes(top->ROM, 12, create_LH(Register::E::x1, 8, Register::E::x2));

      top->rst = 0;
      clock_cycle();
      clock_cycle();
      top->rst = 1;
      EXECUTE_INSTR;

      ASSERT_EQUALS(top->REGISTERS[1], 0x1000);
      EXECUTE_INSTR;

      ASSERT_EQUALS(top->REGISTERS[0], 0x00005678);
      EXECUTE_INSTR;

      ASSERT_EQUALS(top->REGISTERS[0], 0xFFFFFFFF);
      EXECUTE_INSTR;

      ASSERT_EQUALS(top->REGISTERS[0], 0x00004321);
    }
};

class TestVcpuLhu: public GeneralTest<Vcpu> {
  public:
    virtual void test() {
      insert_4bytes(top->RAM, 3, -1);
      insert_4bytes(top->ROM, 0, create_LUI(Register::E::x2, 0x00001));
      insert_4bytes(top->ROM, 4, create_LHU(Register::E::x1, 3, Register::E::x2));

      top->rst = 0;
      clock_cycle();
      clock_cycle();
      top->rst = 1;
      EXECUTE_INSTR;

      ASSERT_EQUALS(top->REGISTERS[1], 0x1000);
      EXECUTE_INSTR;

      // storing -1 (or 0xffffffff) into 32bit and reading only 8 bit unsigned,
      // stuffing it into 32bit makes this 0xff
      ASSERT_EQUALS(top->REGISTERS[0], 0xffff);
    }
};

class TestVcpuLw: public GeneralTest<Vcpu> {
  public:
    virtual void test() {
      insert_4bytes(top->RAM, 0, 0x12345678);
      insert_4bytes(top->RAM, 4, 0xC0FFEE23);
      insert_4bytes(top->ROM, 0, create_LUI(Register::E::x2, 0x00001));
      insert_4bytes(top->ROM, 4, create_LW(Register::E::x1, 0, Register::E::x2));
      insert_4bytes(top->ROM, 8, create_LW(Register::E::x1, 4, Register::E::x2));
      insert_4bytes(top->ROM, 12, create_LW(Register::E::x1, 2, Register::E::x2));

      top->rst = 0;
      clock_cycle();
      clock_cycle();
      top->rst = 1;
      EXECUTE_INSTR;

      ASSERT_EQUALS(top->REGISTERS[1], 0x1000);
      EXECUTE_INSTR;

      ASSERT_EQUALS(top->REGISTERS[0], 0x12345678);
      EXECUTE_INSTR;

      ASSERT_EQUALS(top->REGISTERS[0], 0xC0FFEE23);
      EXECUTE_INSTR;

      ASSERT_EQUALS(top->REGISTERS[0], 0xEE231234);
    }
};

class TestVcpuSw: public GeneralTest<Vcpu> {
  public:
    virtual void test() {
      top->RAM[0] = 0xCA;
      top->RAM[5] = 0xCA;
      insert_4bytes(top->ROM, 0, create_LUI(Register::E::x1, 0x00001));
      insert_4bytes(top->ROM, 4, create_LUI(Register::E::x2, 0x87654));
      insert_4bytes(top->ROM, 8, create_ADDI(Register::E::x2, Register::E::x2, 801));
      // sw x2, 1(x1)
      insert_4bytes(top->ROM, 12, create_SW(Register::E::x2, 1, Register::E::x1));
      // sw x3, 1(x1)
      insert_4bytes(top->ROM, 16, create_LW(Register::E::x3, 1, Register::E::x1));

      top->rst = 0;
      clock_cycle();
      clock_cycle();
      top->rst = 1;
      EXECUTE_INSTR;

      ASSERT_EQUALS(top->REGISTERS[0], 0x1000);
      EXECUTE_INSTR;

      ASSERT_EQUALS(top->REGISTERS[1], 0x87654000);
      EXECUTE_INSTR;

      ASSERT_EQUALS(top->REGISTERS[1], 0x87654321);
      EXECUTE_INSTR;

      ASSERT_EQUALS(top->RAM[0], 0xCA);
      ASSERT_EQUALS(top->RAM[5], 0xCA);
      ASSERT_EQUALS(top->RAM[1], 0x21);
      ASSERT_EQUALS(top->RAM[2], 0x43);
      ASSERT_EQUALS(top->RAM[3], 0x65);
      ASSERT_EQUALS(top->RAM[4], 0x87);
      EXECUTE_INSTR;

      ASSERT_EQUALS(top->REGISTERS[2], 0x87654321);
    }
};

class TestVcpuSh: public GeneralTest<Vcpu> {
  public:
    virtual void test() {
      top->REGISTERS[0] = 0x1000;
      top->REGISTERS[1] = 0x87654321;
      top->REGISTERS[2] = 0x12345678;

      top->RAM[0] = 0xCA;
      top->RAM[1] = 0xCA;
      top->RAM[2] = 0xCA;
      top->RAM[3] = 0xCA;
      // sh x2, 1(x1)
      insert_4bytes(top->ROM, 0, create_SH(Register::E::x2, 1, Register::E::x1));
      // lh x3, 1(x1)
      insert_4bytes(top->ROM, 4, create_LH(Register::E::x3, 1, Register::E::x1));

      top->rst = 0;
      clock_cycle();
      clock_cycle();
      top->rst = 1;
      EXECUTE_INSTR;

      ASSERT_EQUALS(top->RAM[0], 0xCA);
      ASSERT_EQUALS(top->RAM[1], 0x21);
      ASSERT_EQUALS(top->RAM[2], 0x43);
      ASSERT_EQUALS(top->RAM[3], 0xCA);
      EXECUTE_INSTR;

      ASSERT_EQUALS(top->REGISTERS[2], 0x00004321);
    }
};

class TestVcpuSb: public GeneralTest<Vcpu> {
  public:
    virtual void test() {
      top->REGISTERS[0] = 0x1000;
      top->REGISTERS[1] = 0x87654321;
      top->REGISTERS[2] = 0x12345678;

      top->RAM[0] = 0xCA;
      top->RAM[1] = 0xCA;
      top->RAM[2] = 0xCA;
      // sw x2, 1(x1)
      insert_4bytes(top->ROM, 0, create_SB(Register::E::x2, 1, Register::E::x1));
      // lb x3, 1(x1)
      insert_4bytes(top->ROM, 4, create_LB(Register::E::x3, 1, Register::E::x1));

      top->rst = 0;
      clock_cycle();
      clock_cycle();
      top->rst = 1;
      EXECUTE_INSTR;

      ASSERT_EQUALS(top->RAM[0], 0xCA);
      ASSERT_EQUALS(top->RAM[1], 0x21);
      ASSERT_EQUALS(top->RAM[2], 0xCA);
      EXECUTE_INSTR;

      ASSERT_EQUALS(top->REGISTERS[2], 0x00000021);
    }
};

class TestVcpuStageCounter: public GeneralTest<Vcpu> {
  public:
    virtual void test() {
      for (int i =0; i<512; i++) {
        insert_4bytes(top->ROM, 4 * i, create_ADDI(Register::E::x0, Register::E::x0, 0));
      }
      top->rst = 1;
      ASSERT_EQUALS(top->STAGE, 0);
      clock_cycle();
      ASSERT_EQUALS(top->STAGE, 1);
      clock_cycle();
      ASSERT_EQUALS(top->STAGE, 2);
      clock_cycle();
      ASSERT_EQUALS(top->STAGE, 3);
      clock_cycle();
      ASSERT_EQUALS(top->STAGE, 4);
      for (int i = 0; i < 5; i++) {
        clock_cycle();
        ASSERT_EQUALS(top->STAGE, 0);
        clock_cycle();
        ASSERT_EQUALS(top->STAGE, 1);
        clock_cycle();
        ASSERT_EQUALS(top->STAGE, 2);
        clock_cycle();
        ASSERT_EQUALS(top->STAGE, 3);
        clock_cycle();
        ASSERT_EQUALS(top->STAGE, 4);
      }
  }
};

class TestVcpuBeq: public GeneralTest<Vcpu> {
  public:
    virtual void test() {
      insert_4bytes(top->ROM, 0, create_BEQ(Register::E::x0, Register::E::x0, 8));
      insert_4bytes(top->ROM, 4, create_ADDI(Register::E::x1, Register::E::x0, 23));
      insert_4bytes(top->ROM, 8, create_ADDI(Register::E::x2, Register::E::x0, 42));
      insert_4bytes(top->ROM, 12, create_BEQ(Register::E::x2, Register::E::x0, 8));
      insert_4bytes(top->ROM, 16, create_ADDI(Register::E::x3, Register::E::x0, 65));
      insert_4bytes(top->ROM, 20, create_ADDI(Register::E::x4, Register::E::x0, 56));

      step();
      top->rst = 1;
      ASSERT_EQUALS(top->PC, 0);
      EXECUTE_INSTR;
      ASSERT_EQUALS(top->PC, 8);

      EXECUTE_INSTR;
      ASSERT_EQUALS(top->REGISTERS[1], 42);
      ASSERT_EQUALS(top->PC, 12);

      EXECUTE_INSTR;
      ASSERT_EQUALS(top->PC, 16);

      EXECUTE_INSTR;
      ASSERT_EQUALS(top->REGISTERS[2], 65);
      ASSERT_EQUALS(top->PC, 20);

      EXECUTE_INSTR;
      ASSERT_EQUALS(top->REGISTERS[3], 56);
      ASSERT_EQUALS(top->PC, 24);
    }
};

class TestVcpuBne: public GeneralTest<Vcpu> {
  public:
    virtual void test() {
      insert_4bytes(top->ROM, 0, create_BNE(Register::E::x0, Register::E::x0, 8));
      insert_4bytes(top->ROM, 4, create_ADDI(Register::E::x1, Register::E::x0, 23));
      insert_4bytes(top->ROM, 8, create_ADDI(Register::E::x2, Register::E::x0, 42));
      insert_4bytes(top->ROM, 12, create_BNE(Register::E::x2, Register::E::x0, 8));
      insert_4bytes(top->ROM, 16, create_ADDI(Register::E::x3, Register::E::x0, 65));
      insert_4bytes(top->ROM, 20, create_ADDI(Register::E::x4, Register::E::x0, 56));

      step();
      top->rst = 1;
      ASSERT_EQUALS(top->PC, 0);
      EXECUTE_INSTR;
      ASSERT_EQUALS(top->PC, 4);

      EXECUTE_INSTR;
      ASSERT_EQUALS(top->REGISTERS[0], 23);
      ASSERT_EQUALS(top->PC, 8);

      EXECUTE_INSTR;
      ASSERT_EQUALS(top->REGISTERS[1], 42);
      ASSERT_EQUALS(top->PC, 12);

      EXECUTE_INSTR;
      ASSERT_EQUALS(top->PC, 20);

      EXECUTE_INSTR;
      ASSERT_EQUALS(top->REGISTERS[3], 56);
      ASSERT_EQUALS(top->PC, 24);
    }
};

class TestVcpuSrai: public GeneralTest<Vcpu> {
  public:
    virtual void test() {
      insert_4bytes(top->ROM, 0, create_LUI(Register::E::x1, 524288));
      insert_4bytes(top->ROM, 4, create_ADDI(Register::E::x1, Register::E::x1, 2));
      insert_4bytes(top->ROM, 8, create_SRAI(1, Register::E::x1, Register::E::x2));

      step();
      top->rst = 1;
      ASSERT_EQUALS(top->PC, 0);
      EXECUTE_INSTR;
      ASSERT_EQUALS(top->PC, 4);

      EXECUTE_INSTR;
      ASSERT_EQUALS(top->REGISTERS[0], 0b10000000000000000000000000000010);
      ASSERT_EQUALS(top->PC, 8);

      EXECUTE_INSTR;
      ASSERT_EQUALS(top->REGISTERS[1], 0b11000000000000000000000000000001);
    }
};

class TestVcpuSrli: public GeneralTest<Vcpu> {
  public:
    virtual void test() {
      insert_4bytes(top->ROM, 0, create_LUI(Register::E::x1, 524288));
      insert_4bytes(top->ROM, 4, create_ADDI(Register::E::x1, Register::E::x1, 2));
      insert_4bytes(top->ROM, 8, create_SRLI(1, Register::E::x1, Register::E::x2));

      step();
      top->rst = 1;
      ASSERT_EQUALS(top->PC, 0);
      EXECUTE_INSTR;
      ASSERT_EQUALS(top->PC, 4);

      EXECUTE_INSTR;
      ASSERT_EQUALS(top->REGISTERS[0], 0b10000000000000000000000000000010);
      ASSERT_EQUALS(top->PC, 8);

      EXECUTE_INSTR;
      ASSERT_EQUALS(top->REGISTERS[1], 0b01000000000000000000000000000001);
    }
};

class TestVcpuSlli: public GeneralTest<Vcpu> {
  public:
    virtual void test() {
      insert_4bytes(top->ROM, 0, create_LUI(Register::E::x1, 524288));
      insert_4bytes(top->ROM, 4, create_ADDI(Register::E::x1, Register::E::x1, 2));
      insert_4bytes(top->ROM, 8, create_SLLI(1, Register::E::x1, Register::E::x2));

      step();
      top->rst = 1;
      ASSERT_EQUALS(top->PC, 0);
      EXECUTE_INSTR;
      ASSERT_EQUALS(top->PC, 4);

      EXECUTE_INSTR;
      ASSERT_EQUALS(top->REGISTERS[0], 0b10000000000000000000000000000010);
      ASSERT_EQUALS(top->PC, 8);

      EXECUTE_INSTR;
      ASSERT_EQUALS(top->REGISTERS[1], 0b00000000000000000000000000000100);
    }
};

class TestVcpuJalr: public GeneralTest<Vcpu> {
  public:
    virtual void test() {
      insert_4bytes(top->ROM, 0, create_ADDI(Register::E::x1, Register::E::x0, 16));
      insert_4bytes(top->ROM, 4, create_JALR(Register::E::x2, 8, Register::E::x1));

      step();
      top->rst = 1;

      EXECUTE_INSTR;
      ASSERT_EQUALS(top->PC, 4);
      ASSERT_EQUALS(top->REGISTERS[0], 16);

      EXECUTE_INSTR;
      ASSERT_EQUALS(top->PC, 24);
      ASSERT_EQUALS(top->REGISTERS[1], 8);
    }
};

class TestVcpuJal: public GeneralTest<Vcpu> {
  public:
    virtual void test() {
      insert_4bytes(top->ROM, 0, create_JAL(Register::E::x2, 8));
      insert_4bytes(top->ROM, 4, create_ADDI(Register::E::x0, Register::E::x0, 0));
      insert_4bytes(top->ROM, 8, create_ADDI(Register::E::x0, Register::E::x0, 0));

      step();
      top->rst = 1;

      EXECUTE_INSTR;
      ASSERT_EQUALS(top->PC, 8);
      ASSERT_EQUALS(top->REGISTERS[1], 4);
    }
};

int main(const int argc, char** argv) {
  cout << "---- CPU RISCV tests passed" << endl;
//  (new TestVcpuStageCounter())->run("vcds/cpu_stage_counter.vcd");
  (new TestVcpuAddi())->run("vcds/cpu_addis.vcd");
  (new TestVcpuAdd())->run("vcds/cpu_adds.vcd");
  (new TestVcpuSub())->run("vcds/cpu_subs.vcd");
  (new TestVcpuLi())->run("vcds/cpu_li.vcd");
  (new TestVcpuAuipc())->run("vcds/cpu_auipc.vcd");
  (new TestVcpuLb_no_signextend())->run("vcds/cpu_lb_no_signextend.vcd");
  (new TestVcpuLb_signextend())->run("vcds/cpu_lb_signextend.vcd");
  (new TestVcpuLbu())->run("vcds/cpu_lbu.vcd");
  (new TestVcpuLh())->run("vcds/cpu_lb.vcd");
  (new TestVcpuLhu())->run("vcds/cpu_lbu.vcd");
  (new TestVcpuLw())->run("vcds/cpu_lw.vcd");
  (new TestVcpuSb())->run("vcds/cpu_sb.vcd");
  (new TestVcpuSh())->run("vcds/cpu_sh.vcd");
  (new TestVcpuSw())->run("vcds/cpu_sw.vcd");
  (new TestVcpuBeq())->run("vcds/cpu_beq.vcd");
  (new TestVcpuBne())->run("vcds/cpu_bne.vcd");
  (new TestVcpuSrai())->run("vcds/cpu_srai.vcd");
  (new TestVcpuSrli())->run("vcds/cpu_srli.vcd");
  (new TestVcpuSlli())->run("vcds/cpu_slli.vcd");
  (new TestVcpuJalr())->run("vcds/cpu_jalr.vcd");
  (new TestVcpuJal())->run("vcds/cpu_jal.vcd");
  cout << "$$$$ CPU RISCV tests passed" << endl;
  HANDLE_ERROR_COUNTER;
}
