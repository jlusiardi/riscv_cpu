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

      assert(top->read_data_0 == expect);
    }

    virtual void test() {
      perform_test_for(2, 538, 538);
/*      
      clock_cycle();

      top->write_address = 2;
      top->write_data = 538;
      top->write_enable = 1;

      clock_cycle();

      top->read_address_0 = 2;

      clock_cycle();

      assert(top->read_data_0 == 538);

      top->write_address = 15;
      top->write_data = 3;

      clock_cycle();

      top->write_enable = 0;
      top->write_address = 0;
      top->read_address_1 = 15;

      clock_cycle();

      assert(top->read_data_1 == 3);

      top->write_enable = 0;

      top->write_address = 15;
      top->write_data = 0;

      clock_cycle();

      top->read_address_0 = 15;

      clock_cycle();

      assert(top->read_data_0 == 3);
*/
    }
};

int main(int argc, char** argv) {
  cout << "---- Testing register file" << endl;
  (new TestRegisterFileRiscv())->run("vcds/register_file.vcd");
  cout << "#### Register file tests pass" << endl;
}
