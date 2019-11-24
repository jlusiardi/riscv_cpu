#include <iostream>

#include "verilated.h"
#include "Vmemory_control_synth.h"
#include "general_test.h"
#include "riscv.h"
using namespace std;

#define ROM memory_control_synth__DOT__mem__DOT__rom__DOT__mem
#define RAM memory_synth__DOT__ram__DOT__mem

class TestMemoryControl_ReadLB: public GeneralTest<Vmemory_control_synth> {
  public:
    virtual void test() {
        top->ROM[0x20] = 0x84;
        top->ROM[0x21] = 0x44;

        top->start=1;
        top->mode=FUNC3_LB;
        top->address=0x20;
        clock_cycle();
        top->start=0;
        clock_cycle();
        ASSERT_EQUALS(top->done, 1);
        ASSERT_EQUALS(top->read_data, 0xFFFFFF84);
        clock_cycle();
        ASSERT_EQUALS(top->done, 0);
        clock_cycle();

        top->start=1;
        top->mode=FUNC3_LB;
        top->address=0x21;
        clock_cycle();
        top->start=0;
        clock_cycle();
        ASSERT_EQUALS(top->done, 1);
        ASSERT_EQUALS(top->read_data, 0x00000044);
        clock_cycle();
        ASSERT_EQUALS(top->done, 0);
        clock_cycle();
    }
};

class TestMemoryControl_ReadLBU: public GeneralTest<Vmemory_control_synth> {
  public:
    virtual void test() {
        top->ROM[0x20] = 0x84;
        top->ROM[0x21] = 0x44;

        top->start=1;
        top->mode=FUNC3_LBU;
        top->address=0x20;
        clock_cycle();
        top->start=0;
        clock_cycle();
        ASSERT_EQUALS(top->done, 1);
        ASSERT_EQUALS(top->read_data, 0x00000084);
        clock_cycle();
        ASSERT_EQUALS(top->done, 0);
        clock_cycle();

        top->start=1;
        top->mode=FUNC3_LBU;
        top->address=0x21;
        clock_cycle();
        top->start=0;
        clock_cycle();
        ASSERT_EQUALS(top->done, 1);
        ASSERT_EQUALS(top->read_data, 0x00000044);
        clock_cycle();
        ASSERT_EQUALS(top->done, 0);
        clock_cycle();
    }
};

class TestMemoryControl_ReadLH: public GeneralTest<Vmemory_control_synth> {
  public:
    virtual void test() {
        top->ROM[0x20] = 0x42; 
        top->ROM[0x21] = 0x23;
        top->ROM[0x22] = 0x42;
        top->ROM[0x23] = 0x84;

        top->start=1;
        top->mode=FUNC3_LH;
        top->address=0x20;
        clock_cycle();
        top->start=0;
        clock_cycle();
        clock_cycle();
        ASSERT_EQUALS(top->done, 1);
        ASSERT_EQUALS(top->read_data, 0x2342);
        clock_cycle();
        ASSERT_EQUALS(top->done, 0);
        clock_cycle();

        top->start=1;
        top->mode=FUNC3_LH;
        top->address=0x22;
        clock_cycle();
        top->start=0;
        clock_cycle();
        clock_cycle();
        ASSERT_EQUALS(top->done, 1);
        ASSERT_EQUALS(top->read_data, 0xFFFF8442);
        clock_cycle();
        ASSERT_EQUALS(top->done, 0);
        clock_cycle();
    }
};

class TestMemoryControl_ReadLHU: public GeneralTest<Vmemory_control_synth> {
  public:
    virtual void test() {
        top->ROM[0x20] = 0x42; 
        top->ROM[0x21] = 0x23;
        top->ROM[0x22] = 0x42;
        top->ROM[0x23] = 0x84;

        top->start=1;
        top->mode=FUNC3_LHU;
        top->address=0x20;
        clock_cycle();
        top->start=0;
        clock_cycle();
        clock_cycle();
        ASSERT_EQUALS(top->done, 1);
        ASSERT_EQUALS(top->read_data, 0x2342);
        clock_cycle();
        ASSERT_EQUALS(top->done, 0);
        clock_cycle();

        top->start=1;
        top->mode=FUNC3_LHU;
        top->address=0x22;
        clock_cycle();
        top->start=0;
        clock_cycle();
        clock_cycle();
        ASSERT_EQUALS(top->done, 1);
        ASSERT_EQUALS(top->read_data, 0x8442);
        clock_cycle();
        ASSERT_EQUALS(top->done, 0);
        clock_cycle();
    }
};

class TestMemoryControl_ReadLW: public GeneralTest<Vmemory_control_synth> {
  public:
    virtual void test() {
        top->ROM[0x20] = 0x42; 
        top->ROM[0x21] = 0x23;
        top->ROM[0x22] = 0x42;
        top->ROM[0x23] = 0x84;

        top->start=1;
        top->mode=FUNC3_LW;
        top->address=0x20;
        clock_cycle();
        top->start=0;
        clock_cycle();
        clock_cycle();
        clock_cycle();
        clock_cycle();
        ASSERT_EQUALS(top->done, 1);
        ASSERT_EQUALS(top->read_data, 0x84422342);
        clock_cycle();
        ASSERT_EQUALS(top->done, 0);
        clock_cycle();
    }
};


int main(int argc, char** argv) {
  cout << "---- Main Memory" << endl;
  (new TestMemoryControl_ReadLB())->run("vcds/memory_control_synth_read_lb.vcd");
  (new TestMemoryControl_ReadLBU())->run("vcds/memory_control_synth_read_lbu.vcd");
  (new TestMemoryControl_ReadLH())->run("vcds/memory_control_synth_read_lh.vcd");
  (new TestMemoryControl_ReadLHU())->run("vcds/memory_control_synth_read_lhu.vcd");
  (new TestMemoryControl_ReadLW())->run("vcds/memory_control_synth_read_lw.vcd");
  cout << "$$$$ Main Memory" << endl;
  HANDLE_ERROR_COUNTER;
}
