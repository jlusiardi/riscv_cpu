#include <iostream>

#include <ctime>
#include <cstdlib>

#include "Vregister_file.h"
#include "verilated.h"
#include "general_test.h"

using namespace std;

class TestRegisterFileRiscv: public GeneralTest<Vregister_file> {
  public:
    void perform_test_for(int wr_add, int data, int expect) {
      clock_cycle();

      top->write_address = wr_add;
      top->write_data = data;
      top->write_enable = 1;

      clock_cycle();

      top->write_enable = 0;
      top->read_address_0 = wr_add;

      clock_cycle();

      ASSERT_EQUALS(top->read_data_0, expect);
    }

    virtual void test() {
      perform_test_for(2, 538, 538);
    }
};

int main(int argc, char** argv) {
  cout << "---- Testing register file" << endl;
  (new TestRegisterFileRiscv())->run("vcds/register_file.vcd");
  cout << "#### Register file tests pass" << endl;
  HANDLE_ERROR_COUNTER;
}
