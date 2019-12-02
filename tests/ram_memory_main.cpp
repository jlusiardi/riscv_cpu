#include <iostream>

#include "verilated.h"
#include "Vram_memory.h"
#include "general_test.h"
#include "riscv.h"

using namespace std;

#define RAM ram_memory__DOT__mem

class TestRamRead: public GeneralTest<Vram_memory> {
  public:
    virtual void test() {
      // initialize ram
      for (int i = 0; i < 4*2048; i++) {
        top->RAM[i] = i % 256;
      }

      for (int i = 0; i < 255; i++) {
        top->address = i;
        clock_cycle();
        ASSERT_EQUALS(top->read_data, i);
        ASSERT_EQUALS(top->illegal_address, 0);
      }

      top->address = 0xFFFFFFFF;
      clock_cycle();
      ASSERT_EQUALS(top->illegal_address, 1);
    }
};

class TestRamWrite: public GeneralTest<Vram_memory> {
  public:
    virtual void test() {
      // initialize ram
      for (int i = 0; i < 4*2048; i++) {
        top->RAM[i] = 0;
      }

      top->write_enable = 0;
      for (int i = 0; i < 255; i++) {
        top->address = i;
        top->write_data = i;
        clock_cycle();
        ASSERT_EQUALS(top->illegal_address, 0);
      }
      
      for (int i = 0; i < 255; i++) {
        ASSERT_EQUALS(top->RAM[i], 0);
      }

      top->write_enable = 1;
      for (int i = 0; i < 255; i++) {
        top->address = i;
        top->write_data = i;
        clock_cycle();
        ASSERT_EQUALS(top->illegal_address, 0);
      }
      
      for (int i = 0; i < 255; i++) {
        ASSERT_EQUALS(top->RAM[i], i);
      }

      top->address = 0xFFFFFFFF;
      clock_cycle();
      ASSERT_EQUALS(top->illegal_address, 1);
    }
};

int main(int argc, char** argv) {
  cout << "---- RAM Memory tests" << endl;
  (new TestRamRead())->run("vcds/ram_read.vcd");
  (new TestRamWrite())->run("vcds/ram_write.vcd");
  cout << "$$$$ RAM Memory" << endl;
}
