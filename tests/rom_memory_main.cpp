#include <iostream>

#include "verilated.h"
#include "Vrom_memory.h"
#include "general_test.h"
#include "riscv.h"

using namespace std;

#define ROM rom_memory__DOT__rom

class TestRomLegalR: public GeneralTest<Vrom_memory> {
  public:
    virtual void test() {
      // initialize rom
      for (int i = 0; i < 512; i++) {
        insert_4bytes(top->ROM, 4 * i, i);
      }

      for (int i = 0; i < 512; i++) {
        top->read_address = 4*i;
        step();
        ASSERT_EQUALS(top->read_data, i);
        ASSERT_EQUALS(top->illegal_read_address, 0);
      }

      top->read_address = 1;
      step();
      ASSERT_EQUALS(top->read_data, 0x01000000);
      ASSERT_EQUALS(top->illegal_read_address, 0);

      top->read_address = 5;
      step();
      ASSERT_EQUALS(top->read_data, 0x02000000);
      ASSERT_EQUALS(top->illegal_read_address, 0);
    }
};

class TestRomIllegalR: public GeneralTest<Vrom_memory> {
  public:
    virtual void test() {
      // assuming a capacity of 512 32bit words, the range is from 0 to 2047 for bytes.
      top->read_address = 4 * 512;
      step();
      ASSERT_EQUALS(top->illegal_read_address, 1);
    }
};

int main(int argc, char** argv) {
  cout << "---- ROM tests" << endl;
  (new TestRomLegalR())->run("vcds/rom_legalr.vcd");
  (new TestRomIllegalR())->run("vcds/rom_illegalr.vcd");
  cout << "$$$$ ROM Memory" << endl;
  HANDLE_ERROR_COUNTER;
}
