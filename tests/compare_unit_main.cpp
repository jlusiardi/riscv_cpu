#include <bitset>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Vcompare_unit.h"
#include "general_test.h"
#include "riscv.h"
#include "verilated.h"

using namespace std;

class TestCompareUnitBEQ : public GeneralTest<Vcompare_unit> {
public:
  virtual void test() {
    top->rs1_value = 23;
    top->rs2_value = 23;
    top->cmp_op = FUNC3_BEQ;
    step();
    assert(top->jump_condition == 0b1);
    
    top->rs1_value = 23;
    top->rs2_value = 42;
    step();
    assert(top->jump_condition == 0b0);
    
  }
};

class TestCompareUnitBNE : public GeneralTest<Vcompare_unit> {
public:
  virtual void test() {
    top->rs1_value = 23;
    top->rs2_value = 23;
    top->cmp_op = FUNC3_BNE;
    step();
    assert(top->jump_condition == 0b0);
    
    top->rs1_value = 23;
    top->rs2_value = 42;
    step();
    assert(top->jump_condition == 0b1);
    
  }
};

class TestCompareUnitBLT : public GeneralTest<Vcompare_unit> {
public:
  virtual void test() {
    top->rs1_value = -23;
    top->rs2_value = 23;
    top->cmp_op = FUNC3_BLT;
    step();
    assert(top->jump_condition == 0b1);
    
    top->rs1_value = 13;
    top->rs2_value = 23;
    step();
    assert(top->jump_condition == 0b1);
    
    top->rs1_value = 23;
    top->rs2_value = 23;
    step();
    assert(top->jump_condition == 0b0);
    
    
    top->rs1_value = 23;
    top->rs2_value = 42;
    step();
    assert(top->jump_condition == 0b1);
    
  }
};

class TestCompareUnitBLTU : public GeneralTest<Vcompare_unit> {
public:
  virtual void test() {
    top->rs1_value = -23;
    top->rs2_value = 23;
    top->cmp_op = FUNC3_BLTU;
    step();
    assert(top->jump_condition == 0b0);
    
    top->rs1_value = 23;
    top->rs2_value = 23;
    step();
    assert(top->jump_condition == 0b0);
    
    
    top->rs1_value = 23;
    top->rs2_value = 42;
    step();
    assert(top->jump_condition == 0b1);
  }
};

class TestCompareUnitBGE : public GeneralTest<Vcompare_unit> {
public:
  virtual void test() {
    top->rs1_value = 23;
    top->rs2_value = -23;
    top->cmp_op = FUNC3_BGE;
    step();
    assert(top->jump_condition == 0b1);
    
    top->rs1_value = 23;
    top->rs2_value = 13;
    step();
    assert(top->jump_condition == 0b1);
    
    top->rs1_value = 23;
    top->rs2_value = 23;
    step();
    assert(top->jump_condition == 0b1);
    
    
    top->rs1_value = 23;
    top->rs2_value = 42;
    step();
    assert(top->jump_condition == 0b0);
    
  }
};

class TestCompareUnitBGEU : public GeneralTest<Vcompare_unit> {
public:
  virtual void test() {
    top->rs1_value = 23;
    top->rs2_value = -23;
    top->cmp_op = FUNC3_BGEU;
    step();
    assert(top->jump_condition == 0b0);
    
    top->rs1_value = 23;
    top->rs2_value = 23;
    step();
    assert(top->jump_condition == 0b1);
    
    
    top->rs1_value = 42;
    top->rs2_value = 23;
    step();
    assert(top->jump_condition == 0b1);
  }
};

int main(int argc, char **argv) {
  cout << "---- Testing compare_unit" << endl;
  (new TestCompareUnitBEQ())->run("vcds/compare_unit_beq.vcd");
  (new TestCompareUnitBNE())->run("vcds/compare_unit_bne.vcd");
  (new TestCompareUnitBLT())->run("vcds/compare_unit_blt.vcd");
  (new TestCompareUnitBLTU())->run("vcds/compare_unit_bltu.vcd");
  (new TestCompareUnitBGE())->run("vcds/compare_unit_bge.vcd");
  (new TestCompareUnitBGEU())->run("vcds/compare_unit_bgeu.vcd");
  cout << "$$$$ compare_unit passes tests" << endl;
}
