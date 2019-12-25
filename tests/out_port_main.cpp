#include <iostream>
#include <bitset>

#include "verilated.h"
#include "Vout_port.h"
#include "general_test.h"
#include "riscv.h"
#include "OutPortTest.h"

using namespace std;


class TestOutPort: public GeneralTest<Vout_port> {
  protected:
    OutPortTest<Vout_port>* outportTest;

  public:
    TestOutPort() {
      outportTest = new OutPortTest<Vout_port>(this->top);
      this->addHardwareAddonTest(this->outportTest);
    }

    virtual void test() {
      clock_cycle();
      top->write_data = 0xF0;
      top->write_enable = 1;
      top->rst = 1;
      clock_cycle();
      ASSERT_EQUALS(top->port, 0xF0);
      top->write_enable = 0;
      clock_cycle();
      ASSERT_EQUALS(top->port, 0xF0);
      top->write_data = 0x0F;
      clock_cycle();
      ASSERT_EQUALS(top->port, 0xF0);
      clock_cycle();
      ASSERT_EQUALS(top->port, 0xF0);
    }
};

int main(const int argc, char** argv) {
  cout << "---- out port tests started" << endl;
  (new TestOutPort())->run("vcds/out_port.vcd");
  cout << "$$$$ out port tests passed" << endl;
  HANDLE_ERROR_COUNTER;
}
