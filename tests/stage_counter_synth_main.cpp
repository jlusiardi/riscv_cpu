#include <iostream>
#include <bitset>

#include "verilated.h"
#include "Vstage_counter_synth.h"
#include "general_test.h"
#include "riscv.h"

using namespace std;


class TestVstage_counter_synth: public GeneralTest<Vstage_counter_synth> {
  public:
    virtual void test() {
      top->rst = 0;
      clock_cycle();
      clock_cycle();
      top->rst = 1;
      clock_cycle();
      top->blocked = 1;
      clock_cycle();
      clock_cycle();
      clock_cycle();
      clock_cycle();
      top->blocked = 0;
      clock_cycle();
      clock_cycle();
      clock_cycle();
      clock_cycle();
      clock_cycle();
      clock_cycle();
    }
};

int main(const int argc, char** argv) {
  cout << "---- CPU RISCV tests passed" << endl;
  (new TestVstage_counter_synth())->run("vcds/stage_counter_synth.vcd");
  cout << "$$$$ CPU RISCV tests passed" << endl;
  HANDLE_ERROR_COUNTER;
}
