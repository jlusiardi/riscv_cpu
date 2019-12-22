#include <iostream>

#include "verilated.h"
#include "Vtx_uart.h"
#include "general_test.h"
#include "riscv.h"
#include "uart.h"

using namespace std;

#define ROM rom_memory__DOT__mem

class TestTxUart: public SerialTest<Vtx_uart> {
  public:
    virtual void test() {
      top->write_enable = 0;
      clock_cycle();
      top->config_data = BAUD_500000;
      top->config_enable = 1;
      clock_cycle();
      top->config_enable = 0;
      clock_cycle();

      top->write_data = 'H';
      top->write_enable = 1;
      clock_cycle();
      top->write_enable = 0;
      for(int i = 0; i < 100 * 15; i++) {
        clock_cycle();
      }
      ASSERT_EQUALS_STRING(this->received, "H");
      this->reset_received();

      top->write_data = '!';
      top->write_enable = 1;
      clock_cycle();
      top->write_enable = 0;
      for(int i = 0; i < 100 * 15; i++) {
        clock_cycle();
      }
      ASSERT_EQUALS_STRING(this->received, "!");
    }
};

int main(int argc, char** argv) {
  cout << "---- TX Uart tests" << endl;
  (new TestTxUart())->run("vcds/tx_uart.vcd");
  cout << "$$$$ TX Uart Memory" << endl;
  HANDLE_ERROR_COUNTER;
}
