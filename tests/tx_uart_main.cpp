#include <iostream>

#include "verilated.h"
#include "Vtx_uart.h"
#include "general_test.h"
#include "riscv.h"

using namespace std;

#define ROM rom_memory__DOT__mem

class TestTxUart: public GeneralTest<Vtx_uart> {
  public:
    virtual void test() {
      clock_cycle();
      top->write_data = 2;
      top->write_enable = 1;
      top->cycles = 2;
      clock_cycle();
      top->write_enable = 0;
      for(int i = 0; i < top->cycles * 15; i++) {
        clock_cycle();
      }

      top->write_data = 'H';
      top->write_enable = 1;
      top->cycles = 2;
      clock_cycle();
      top->write_enable = 0;
      for(int i = 0; i < top->cycles * 15; i++) {
        clock_cycle();
      }

    }
};

int main(int argc, char** argv) {
  cout << "---- TX Uart tests" << endl;
  (new TestTxUart())->run("vcds/tx_uart.vcd");
  cout << "$$$$ TX Uart Memory" << endl;
  HANDLE_ERROR_COUNTER;
}
