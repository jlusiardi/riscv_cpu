#include <iostream>

#include "verilated.h"
#include "Vmemory.h"
#include "general_test.h"
#include "riscv.h"


#define ROM memory__DOT__rom___DOT__rom
#define RAM memory__DOT__ram__DOT__mem

class TestMemory: public GeneralTest<Vmemory> {
  public:
    virtual void test() {
      // initialize rom
      for (int i = 0; i < 512; i++) {
        insert_4bytes(top->ROM, 4 * i, i);
      }

      // initialize ram
      for (int i = 0; i < 1024; i++) {
        insert_4bytes(top->RAM, 4 * i, i);
      }

      // write to first ram address and read illegal address
      top->write_enable = 1;
      top->size_and_sign = FUNC3_SW;
      top->write_address = 4096;
      top->write_data = 0xC0FFEE23;
      top->read_address = 4095;
      clock_cycle();
      ASSERT_EQUALS(top->illegal_write_address, 0);
      ASSERT_EQUALS(top->illegal_read_address, 1);
      
      // read the ram
      top->write_enable = 0;
      top->read_address = 4096;
      top->size_and_sign = FUNC3_LW;
      clock_cycle();
      ASSERT_EQUALS(top->illegal_read_address, 0);
      ASSERT_EQUALS(top->read_data, 0xC0FFEE23);

      // read from rom
      top->read_address = 4;
      clock_cycle();
      ASSERT_EQUALS(top->illegal_read_address, 0);
      ASSERT_EQUALS(top->read_data, 0x00000001);

      // both adresses are illegal
      top->write_enable = 1;
      top->write_address = 4095;
      top->write_data = 0xC0FFEE23;
      top->read_address = 4095;
      clock_cycle();
      ASSERT_EQUALS(top->illegal_write_address, 1);
      ASSERT_EQUALS(top->illegal_read_address, 1);
    }
};

using namespace std;

int main(int argc, char** argv) {
  cout << "---- Main Memory" << endl;
  (new TestMemory())->run("vcds/memory.vcd");
  cout << "$$$$ Main Memory" << endl;
  HANDLE_ERROR_COUNTER;
}
