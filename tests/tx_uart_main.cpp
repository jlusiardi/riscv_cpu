#include <iostream>

#include "verilated.h"
#include "Vtx_uart.h"
#include "general_test.h"
#include "riscv.h"
#include "uart.h"
#include "SerialTest.h"

using namespace std;

#define ROM rom_memory__DOT__mem

class TestTxUart: public GeneralTest<Vtx_uart> {
  protected:
    SerialTest<Vtx_uart>* serialTest;

  public:
    TestTxUart() {
      serialTest = new SerialTest<Vtx_uart>(this->top);
      this->addHardwareAddonTest(this->serialTest);
    }

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
      ASSERT_EQUALS_STRING(this->serialTest->get_received(), "H");
      this->serialTest->reset_received();

      top->write_data = '!';
      top->write_enable = 1;
      clock_cycle();
      top->write_enable = 0;
      for(int i = 0; i < 100 * 15; i++) {
        clock_cycle();
      }
      ASSERT_EQUALS_STRING(this->serialTest->get_received(), "!");
    }
};

int main(int argc, char** argv) {
  cout << "---- TX Uart tests" << endl;
  (new TestTxUart())->run("vcds/tx_uart.vcd");
  cout << "$$$$ TX Uart Memory" << endl;
  HANDLE_ERROR_COUNTER;
}
