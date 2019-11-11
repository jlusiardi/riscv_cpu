#include <iostream>

#include "verilated.h"
#include "Vram_memory.h"
#include "general_test.h"
#include "riscv.h"

using namespace std;

#define RAM ram_memory__DOT__mem

class TestRamLegalR: public GeneralTest<Vram_memory> {
  public:
    virtual void test() {
      // initialize ram
      for (int i = 0; i < 1024; i++) {
        insert_4bytes(top->RAM, 4 * i, i);
      }

      for (int i = 0; i < 1024; i++) {
        top->read_address = 4*i;
        step();
        assert(top->read_data == i);
        assert(top->illegal_read_address == 0);
      }

      top->read_address = 1;
      step();
      assert(top->read_data == 0x01000000);
      assert(top->illegal_read_address == 0);

      top->read_address = 5;
      step();
      assert(top->read_data == 0x02000000);
      assert(top->illegal_read_address == 0);
    }
};

class TestMemoryLegalRW: public GeneralTest<Vram_memory> {
  public:
    virtual void test() {
      top->write_enable = 1;
      top->size_and_sign = FUNC3_SW;
      for (int i = 0; i < 1024; i+=4) {
        top->write_address = i;
        top->write_data = 1023 - i;
        clock_cycle();
        assert(top->illegal_write_address == 0);
      }

      top->write_enable = 0;
      for (int i = 0; i < 1024; i+=4) {
        clock_cycle();
        top->read_address = i;
        assert(top->illegal_read_address == 0);
        clock_cycle();
        assert(top->read_data == 1023 - i);
      }
    }
};

class TestMemoryIllegalRW: public GeneralTest<Vram_memory> {
  public:
    virtual void test() {
      top->write_enable = 1;

      clock_cycle();
      top->write_address = 4 * 1024;
      top->read_address = 0;
      top->write_data = 2048;
      assert(top->illegal_read_address == 0);
      clock_cycle();
      assert(top->illegal_write_address == 1);

      top->write_enable = 0;
      clock_cycle();
      top->read_address = 4 * 1024;
      top->write_address = 0;
      clock_cycle();
      assert(top->illegal_read_address == 1);
    }
};

int main(int argc, char** argv) {
  cout << "---- Main Memory tests" << endl;
  (new TestRamLegalR())->run("vcds/ram_legalr.vcd");
  (new TestMemoryLegalRW())->run("vcds/memory_legalrw.vcd");
  (new TestMemoryIllegalRW())->run("vcds/memory_illegalrw.vcd");
  cout << "$$$$ Main Memory" << endl;
}
