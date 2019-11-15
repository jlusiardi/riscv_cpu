#include <iostream>

#include "verilated.h"
#include "Vmemory_control.h"
#include "general_test.h"
#include "riscv.h"


class TestMemoryControl: public GeneralTest<Vmemory_control> {
  public:
    virtual void test() {
      // in fetch stage, only the pc is relevant
      top->stage = STAGE_FETCH;
      top->pc_value = 0x00000004;
      top->offset = 0x00000040;
      top->rs1_value = 0x00000040;
      step();
      assert(top->read_address == 0x00000004);
      assert(top->write_enable == 0);

      // in memory stage, the read and write address is computed from offset
      // and rs1_value
      top->stage = STAGE_MEMORY;
      top->opcode = Opcode::E::STORE;
      top->pc_value = 0x00000004;
      top->offset = 0x00000030;
      top->rs1_value = 0x00000010;
      top->memory_write_data = 0x00000042;
      step();
      assert(top->read_address == 0x00000040);
      assert(top->write_address == 0x00000040);
      assert(top->write_data == 0x00000042);
      assert(top->write_enable == 1);

      top->opcode = Opcode::E::LOAD;
      top->pc_value = 0x00000004;
      top->offset = 0x00000030;
      top->rs1_value = 0x00000010;
      top->memory_write_data = 0x00000042;
      step();
      assert(top->read_address == 0x00000040);
      assert(top->write_address == 0x00000040);
      assert(top->write_data == 0x00000042);
      assert(top->write_enable == 0);

   }
};

using namespace std;

int main(int argc, char** argv) {
  cout << "---- Main Memory Control" << endl;
  (new TestMemoryControl())->run("vcds/memory_controll.vcd");
  cout << "$$$$ Main Memory Control" << endl;
}
