#include <iostream>
#include <bitset>

#include "verilated.h"
#include "Vcsr.h"
#include "general_test.h"
#include "riscv.h"

using namespace std;

#define CSRS csr__DOT__csrs

class TestVcsrrw: public GeneralTest<Vcsr> {
  public:
    virtual void test() {
      top->CSRS[CSR::E::MSCRATCH] = 0x42000023;
      top->input_value = 0x23000042;
      top->operation = SystemFunc3::E::CSRRW;
      top->csr_number = CSR::E::MSCRATCH;
      top->write_enable = 1;
      top->rs1 = 1;

      clock_cycle();

      ASSERT_EQUALS(top->csr_value, 0x42000023);
      ASSERT_EQUALS(top->CSRS[CSR::E::MSCRATCH], 0x23000042);
    }
};

class TestVcsrrwReg0: public GeneralTest<Vcsr> {
  public:
    virtual void test() {
      top->CSRS[CSR::E::MSCRATCH] = 0x42000023;
      top->input_value = 0x23000042;
      top->operation = SystemFunc3::E::CSRRW;
      top->csr_number = CSR::E::MSCRATCH;
      top->write_enable = 1;
      top->rs1 = 0;

      clock_cycle();

      ASSERT_EQUALS(top->csr_value, 0x42000023);
      ASSERT_EQUALS(top->CSRS[CSR::E::MSCRATCH], 0x42000023);
    }
};

class TestVcsrrs: public GeneralTest<Vcsr> {
  public:
    virtual void test() {
      top->CSRS[CSR::E::MSCRATCH] = 0x42000023;
      top->operation = SystemFunc3::E::CSRRS;
      top->csr_number = CSR::E::MSCRATCH;
      top->input_value = 0x00FFFF00;
      top->write_enable = 1;
      top->rs1 = 1;

      clock_cycle();

      ASSERT_EQUALS(top->csr_value, 0x42000023);
      ASSERT_EQUALS(top->CSRS[CSR::E::MSCRATCH], 0x42FFFF23);
    }
};

class TestVcsrrsReg0: public GeneralTest<Vcsr> {
  public:
    virtual void test() {
      top->CSRS[CSR::E::MSCRATCH] = 0x42000023;
      top->operation = SystemFunc3::E::CSRRS;
      top->csr_number = CSR::E::MSCRATCH;
      top->input_value = 0x00FFFF00;
      top->write_enable = 1;
      top->rs1 = 0;

      clock_cycle();

      ASSERT_EQUALS(top->csr_value, 0x42000023);
      ASSERT_EQUALS(top->CSRS[CSR::E::MSCRATCH], 0x42000023);
    }
};

class TestVcsrrc: public GeneralTest<Vcsr> {
  public:
    virtual void test() {
      top->CSRS[CSR::E::MSCRATCH] = 0x42000023;
      top->operation = SystemFunc3::E::CSRRC;
      top->csr_number = CSR::E::MSCRATCH;
      top->input_value = 0x0000FFFF;
      top->write_enable = 1;
      top->rs1 = 1;

      clock_cycle();

      ASSERT_EQUALS(top->csr_value, 0x42000023);
      ASSERT_EQUALS(top->CSRS[CSR::E::MSCRATCH], 0x42000000);
    }
};

class TestVcsrrcReg0: public GeneralTest<Vcsr> {
  public:
    virtual void test() {
      top->CSRS[CSR::E::MSCRATCH] = 0x42000023;
      top->operation = SystemFunc3::E::CSRRC;
      top->csr_number = CSR::E::MSCRATCH;
      top->input_value = 0x0000FFFF;
      top->write_enable = 1;
      top->rs1 = 0;

      clock_cycle();

      ASSERT_EQUALS(top->csr_value, 0x42000023);
      ASSERT_EQUALS(top->CSRS[CSR::E::MSCRATCH], 0x42000023);
    }
};

int main(const int argc, char** argv) {
  cout << "---- CSR tests passed" << endl;
  (new TestVcsrrw())->run("vcds/csr_csrrw.vcd");
  (new TestVcsrrwReg0())->run("vcds/csr_csrrw_reg0.vcd");
  (new TestVcsrrs())->run("vcds/csr_csrrs.vcd");
  (new TestVcsrrsReg0())->run("vcds/csr_csrrs_reg0.vcd");
  (new TestVcsrrc())->run("vcds/csr_csrrc.vcd");
  cout << "$$$$ CSR tests passed" << endl;
  HANDLE_ERROR_COUNTER;
}
