#include <iostream>
#include <bitset>

#include "verilated.h"
#include "Vstage_counter.h"
#include "general_test.h"
#include "riscv.h"

using namespace std;


class TestVstage_counter: public GeneralTest<Vstage_counter> {
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
  cout << "---- Stage counter tests started" << endl;
  (new TestVstage_counter())->run("vcds/stage_counter.vcd");
  cout << "$$$$ Stage counter tests passed" << endl;
  HANDLE_ERROR_COUNTER;
}
