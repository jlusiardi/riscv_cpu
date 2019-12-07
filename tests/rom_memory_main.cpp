#include <iostream>

#include "verilated.h"
#include "Vrom_memory.h"
#include "general_test.h"
#include "riscv.h"

using namespace std;

#define ROM rom_memory__DOT__mem

class TestRomLegalR: public GeneralTest<Vrom_memory> {
  public:
    virtual void test() {
      // initialize rom
      for (int i = 0; i < 512; i++) {
        top->ROM[i] = i % 256;
      }

      for (int i = 0; i < 512; i++) {
        top->address = i;
        top->output_enable = 1;
        step();
        ASSERT_EQUALS(top->read_data, i % 256);
        ASSERT_EQUALS(top->illegal_address, 0);
      }
    }
};

class TestRomIllegalR: public GeneralTest<Vrom_memory> {
  public:
    virtual void test() {
      // assuming a capacity of 512 32bit words, the range is from 0 to 2047 for bytes.
      top->address = 4 * 512;
      step();
      ASSERT_EQUALS(top->illegal_address, 1);
    }
};

int main(int argc, char** argv) {
  cout << "---- ROM tests" << endl;
  (new TestRomLegalR())->run("vcds/rom_legalr.vcd");
  (new TestRomIllegalR())->run("vcds/rom_illegalr.vcd");
  cout << "$$$$ ROM Memory" << endl;
  HANDLE_ERROR_COUNTER;
}
