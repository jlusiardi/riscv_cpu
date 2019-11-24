#include <iostream>

#include "verilated.h"
#include "Vmemory_synth.h"
#include "general_test.h"
#include "riscv.h"


#define ROM memory_synth__DOT__rom__DOT__mem
#define RAM memory_synth__DOT__ram__DOT__mem

class TestMemory: public GeneralTest<Vmemory_synth> {
  public:
    virtual void test() {
      // initialize rom
      for (int i = 0; i < 2048; i++) {
        top->ROM[i] = i % 256;
      }

      // initialize ram
      for (int i = 0; i < 8192; i++) {
        top->RAM[i] = i % 256;
      }

      // write to first ram address and read illegal address
      top->write_enable = 1;
      top->address = 0x1000;
      top->write_data = 0x23;
      clock_cycle();
      
      // read the ram
      top->write_enable = 0;
      top->address = 4096;
      clock_cycle();
      ASSERT_EQUALS(top->read_data, 0x23);

      // read from rom
      top->address = 4;
      clock_cycle();
      ASSERT_EQUALS(top->read_data, 0x04);

    }
};

using namespace std;

int main(int argc, char** argv) {
  cout << "---- Main Memory" << endl;
  (new TestMemory())->run("vcds/memory_synth.vcd");
  cout << "$$$$ Main Memory" << endl;
  HANDLE_ERROR_COUNTER;
}
