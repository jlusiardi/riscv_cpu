#include <iostream>

#include "verilated.h"
#include "Vmemory_control.h"
#include "general_test.h"
#include "riscv.h"
using namespace std;

class TestMemoryControl_ReadSB: public GeneralTest<Vmemory_control> {
  public:
    virtual void test() {
        top->rst=1;
        top->start=1;
        top->mode=FUNC3_SB;
        top->address=0x1000;
        top->write_data = 0x00000023;
        clock_cycle();
        ASSERT_EQUALS(top->address_to_mem, 0x1000);
        ASSERT_EQUALS(top->data_to_mem, 0x23);
        ASSERT_EQUALS(top->active, 1);
        top->start=0;
        clock_cycle();
        ASSERT_EQUALS(top->data_to_mem, 0x00);
        ASSERT_EQUALS(top->done, 1);
        ASSERT_EQUALS(top->active, 0);
        clock_cycle();
        ASSERT_EQUALS(top->done, 0);
        clock_cycle();
    }
};

class TestMemoryControl_ReadSH: public GeneralTest<Vmemory_control> {
  public:
    virtual void test() {
        top->rst=1;
        top->start=1;
        top->mode=FUNC3_SH;
        top->address=0x1000;
        top->write_data = 0x00004223;
        clock_cycle();
        ASSERT_EQUALS(top->address_to_mem, 0x1001);
        ASSERT_EQUALS(top->data_to_mem, 0x42);
        ASSERT_EQUALS(top->active, 1);
        top->start=0;
        clock_cycle();
        ASSERT_EQUALS(top->address_to_mem, 0x1000);
        ASSERT_EQUALS(top->data_to_mem, 0x23);
        ASSERT_EQUALS(top->active, 1);
        clock_cycle();
        ASSERT_EQUALS(top->data_to_mem, 0x00);
        ASSERT_EQUALS(top->done, 1);
        ASSERT_EQUALS(top->active, 0);
        clock_cycle();
        ASSERT_EQUALS(top->done, 0);
        clock_cycle();
    }
};

class TestMemoryControl_ReadSW: public GeneralTest<Vmemory_control> {
  public:
    virtual void test() {
        top->rst=1;
        top->start=1;
        top->mode=FUNC3_SW;
        top->address=0x1000;
        top->write_data = 0x87654321;
        clock_cycle();
        ASSERT_EQUALS(top->address_to_mem, 0x1003);
        ASSERT_EQUALS(top->data_to_mem, 0x87);
        ASSERT_EQUALS(top->active, 1);
        top->start=0;
        clock_cycle();
        ASSERT_EQUALS(top->address_to_mem, 0x1002);
        ASSERT_EQUALS(top->data_to_mem, 0x65);
        ASSERT_EQUALS(top->active, 1);
        clock_cycle();
        ASSERT_EQUALS(top->address_to_mem, 0x1001);
        ASSERT_EQUALS(top->data_to_mem, 0x43);
        ASSERT_EQUALS(top->active, 1);
        clock_cycle();
        ASSERT_EQUALS(top->address_to_mem, 0x1000);
        ASSERT_EQUALS(top->data_to_mem, 0x21);
        ASSERT_EQUALS(top->active, 1);
        clock_cycle();
        ASSERT_EQUALS(top->done, 1);
        ASSERT_EQUALS(top->active, 0);
        clock_cycle();
        ASSERT_EQUALS(top->done, 0);
        clock_cycle();
    }
};

class TestMemoryControl_ReadLB: public GeneralTest<Vmemory_control> {
  public:
    virtual void test() {
        top->rst=1;
        top->start=1;
        top->mode=FUNC3_LB;
        top->address=0x20;
        clock_cycle();
        top->start=0;
        top->data_from_mem=0x84;
        ASSERT_EQUALS(top->active, 1);
        clock_cycle();
        ASSERT_EQUALS(top->done, 1);
        ASSERT_EQUALS(top->active, 0);
        ASSERT_EQUALS(top->read_data, 0xFFFFFF84);
        clock_cycle();
        ASSERT_EQUALS(top->done, 0);
        clock_cycle();

        top->start=1;
        top->mode=FUNC3_LB;
        top->address=0x21;
        clock_cycle();
        ASSERT_EQUALS(top->active, 1);
        top->start=0;
        top->data_from_mem=0x44;
        clock_cycle();
        ASSERT_EQUALS(top->done, 1);
        ASSERT_EQUALS(top->active, 0);
        ASSERT_EQUALS(top->read_data, 0x00000044);
        clock_cycle();
        ASSERT_EQUALS(top->done, 0);
        clock_cycle();
    }
};

class TestMemoryControl_ReadLBU: public GeneralTest<Vmemory_control> {
  public:
    virtual void test() {
        top->rst=1;

        top->start=1;
        top->mode=FUNC3_LBU;
        top->address=0x20;
        clock_cycle();
        ASSERT_EQUALS(top->active, 1);
        top->start=0;
        top->data_from_mem=0x84;
        clock_cycle();
        ASSERT_EQUALS(top->done, 1);
        ASSERT_EQUALS(top->active, 0);
        ASSERT_EQUALS(top->read_data, 0x00000084);
        clock_cycle();
        ASSERT_EQUALS(top->done, 0);
        clock_cycle();

        top->start=1;
        top->mode=FUNC3_LBU;
        top->address=0x21;
        clock_cycle();
        ASSERT_EQUALS(top->active, 1);
        top->start=0;
        top->data_from_mem=0x44;
        clock_cycle();
        ASSERT_EQUALS(top->done, 1);
        ASSERT_EQUALS(top->active, 0);
        ASSERT_EQUALS(top->read_data, 0x00000044);
        clock_cycle();
        ASSERT_EQUALS(top->done, 0);
        clock_cycle();
    }
};

class TestMemoryControl_ReadLH: public GeneralTest<Vmemory_control> {
  public:
    virtual void test() {
        top->rst=1;

        top->start=1;
        top->mode=FUNC3_LH;
        top->address=0x20;
        clock_cycle();
        ASSERT_EQUALS(top->active, 1);
        top->start=0;
        top->data_from_mem=0x23;
        clock_cycle();
        ASSERT_EQUALS(top->active, 1);
        top->data_from_mem=0x42;
        clock_cycle();
        ASSERT_EQUALS(top->done, 1);
        ASSERT_EQUALS(top->active, 0);
        ASSERT_EQUALS(top->read_data, 0x2342);
        clock_cycle();
        ASSERT_EQUALS(top->done, 0);
        clock_cycle();

        top->start=1;
        top->mode=FUNC3_LH;
        top->address=0x22;
        clock_cycle();
        ASSERT_EQUALS(top->active, 1);
        top->start=0;
        top->data_from_mem=0x84;
        clock_cycle();
        ASSERT_EQUALS(top->active, 1);
        top->data_from_mem=0x42;
        clock_cycle();
        ASSERT_EQUALS(top->done, 1);
        ASSERT_EQUALS(top->active, 0);
        ASSERT_EQUALS(top->read_data, 0xFFFF8442);
        clock_cycle();
        ASSERT_EQUALS(top->done, 0);
        clock_cycle();
    }
};

class TestMemoryControl_ReadLHU: public GeneralTest<Vmemory_control> {
  public:
    virtual void test() {
        top->rst=1;
        top->start=1;
        top->mode=FUNC3_LHU;
        top->address=0x20;
        clock_cycle();
        ASSERT_EQUALS(top->active, 1);
        top->start=0;
        top->data_from_mem=0x23;
        clock_cycle();
        top->data_from_mem=0x42;
        ASSERT_EQUALS(top->active, 1);
        clock_cycle();
        ASSERT_EQUALS(top->done, 1);
        ASSERT_EQUALS(top->active, 0);
        ASSERT_EQUALS(top->read_data, 0x2342);
        clock_cycle();
        ASSERT_EQUALS(top->done, 0);
        clock_cycle();

        top->start=1;
        top->mode=FUNC3_LHU;
        top->address=0x22;
        clock_cycle();
        ASSERT_EQUALS(top->active, 1);
        top->start=0;
        top->data_from_mem=0x84;
        clock_cycle();
        top->data_from_mem=0x42;
        ASSERT_EQUALS(top->active, 1);
        clock_cycle();
        ASSERT_EQUALS(top->done, 1);
        ASSERT_EQUALS(top->active, 0);
        ASSERT_EQUALS(top->read_data, 0x8442);
        clock_cycle();
        ASSERT_EQUALS(top->done, 0);
        clock_cycle();
    }
};

class TestMemoryControl_ReadLW: public GeneralTest<Vmemory_control> {
  public:
    virtual void test() {
        top->rst=1;
        top->start=1;
        top->mode=FUNC3_LW;
        top->address=0x20;
        clock_cycle();
        ASSERT_EQUALS(top->active, 1);
        top->start=0;
        top->data_from_mem=0x84;
        clock_cycle();
        ASSERT_EQUALS(top->active, 1);
        top->data_from_mem=0x42;
        clock_cycle();
        ASSERT_EQUALS(top->active, 1);
        top->data_from_mem=0x23;
        clock_cycle();
        ASSERT_EQUALS(top->active, 1);
        top->data_from_mem=0x42;
        clock_cycle();
        ASSERT_EQUALS(top->done, 1);
        ASSERT_EQUALS(top->active, 0);
        ASSERT_EQUALS(top->read_data, 0x84422342);
        clock_cycle();
        ASSERT_EQUALS(top->done, 0);
        clock_cycle();
    }
};

int main(int argc, char** argv) {
  cout << "---- Memory Control Tests" << endl;
  (new TestMemoryControl_ReadSB())->run("vcds/memory_control_read_sb.vcd");
  (new TestMemoryControl_ReadSH())->run("vcds/memory_control_read_sh.vcd");
  (new TestMemoryControl_ReadSW())->run("vcds/memory_control_read_sw.vcd");
  (new TestMemoryControl_ReadLB())->run("vcds/memory_control_read_lb.vcd");
  (new TestMemoryControl_ReadLBU())->run("vcds/memory_control_read_lbu.vcd");
  (new TestMemoryControl_ReadLH())->run("vcds/memory_control_read_lh.vcd");
  (new TestMemoryControl_ReadLHU())->run("vcds/memory_control_read_lhu.vcd");
  (new TestMemoryControl_ReadLW())->run("vcds/memory_control_read_lw.vcd");
  cout << "$$$$ Memory Control" << endl;
  HANDLE_ERROR_COUNTER;
}
