#pragma once

#include <verilated_vcd_c.h>
#include <string>
#include <iostream>
#include "riscv.h"

int ERROR_COUNTER = 0;

#define ASSERT_EQUALS(RESULT, EXPECTED) {\
        if (!((RESULT) == (EXPECTED))) {\
            std::cout << "\x1B[31m[FAIL] " << std::hex << (int)(RESULT) << " != " \
                      << std::hex << (EXPECTED) << " (expected) in " << __FILE__ << ":" \
                      << std::dec << __LINE__ << "\x1B[0m" << std::endl;\
            ERROR_COUNTER++;\
        }\
    }

#define ASSERT_EQUALS_STRING(RESULT, EXPECTED) {\
        if (!((RESULT) == (EXPECTED))) {\
            std::cout << "\x1B[31m[FAIL] " << std::hex << RESULT << " != " \
                      << std::hex << (EXPECTED) << " (expected) in " << __FILE__ << ":" \
                      << std::dec << __LINE__ << "\x1B[0m" << std::endl;\
            ERROR_COUNTER++;\
        }\
    }

#define HANDLE_ERROR_COUNTER {\
        if (ERROR_COUNTER > 0) { \
            exit(ERROR_COUNTER); \
        } \
}

/**
 * Macro to execute exactly one instruction by performin clock cycles until the
 * next stage is the fetch stage.
 */
#define EXECUTE_INSTR \
        do {\
            clock_cycle();\
        } while (top->STAGE == STAGE_FETCH);\
        do {\
            clock_cycle();\
        } while (top->STAGE != STAGE_FETCH);\
        count_instruction();

template<class TOP>
class GeneralTest {
  protected:
    TOP* top;
    VerilatedVcdC *trace;
    int time;
    int cycles;
    int instructions;

  public:
    GeneralTest() {
      Verilated::traceEverOn(true);
      top = new TOP; 
      trace = new VerilatedVcdC;
      top->trace(trace, 99);
      this->time = 0;
      this->cycles = 0;
      this->instructions = 0;
    }

    void count_instruction() {
      this->instructions++;
    }

    /**
     * This function is called to perform the test and the tracing.
     * Parameter filename:
     *    name of the file where the VCD will be stored. Defaults to nulltpr,
     *    which means no tracing is done
     * Parameter do_trace:
     *    can be used to turn off tracing by setting to false (event if a
     *    filename was given)
     */
    void run(const char* filename=nullptr, bool do_trace=true) {
      if (filename && do_trace) {
        std::cout << "writing trace to '" << filename << "'." << std::endl;
        trace->open(filename);
      }
      // have the verilator code do the initial blocks first
      step();
      test();
      step();
      top->final();
      if (filename && do_trace) {
        trace->flush();
        trace->close();
      }
    }

    /**
     * Perform one step meaning to eval the state, dump the trace and
     * increment the time.
     */
    void step() {
      top->eval();
      trace->dump(10*time);
      time++;
    }

    /**
     * Perform one clock cycle starting at clk=0.
     */
    void clock_cycle() {
      assert(top->clk == 0);
      step();
      top->clk = 1;
      this->cycles++;
      step();
      top->clk = 0;
    }

    /**
     * Execute n clock cycles.
     */
    void clock_cycles(int n) {
      for (int i = 0; i < n; i++) {
        clock_cycle();
      }
    }

    /**
     * Perform a reset by pulling reset low for one clock cycle
     */
    void reset() {
      top->clk = 0;
      step();
      top->rst = 1;
      top->clk = 0;
      step();
      top->rst = 0;
      top->clk = 0;
      step();
      top->rst = 1;
      top->clk = 0;
      step();
    }

    /**
     *  Implement this function to perform actual test.
     */
    virtual void test() = 0;
};

#define IDLE_BIT -2
#define START_BIT -1
#define FIRST_BIT 0
#define LAST_BIT 7
#define STOP_BIT 8
#define BAUD_500000_CYLCES 101

template<class TOP>
class SerialTest : public GeneralTest<TOP> {
  protected:
    int last_value = -1;
    int next_cycle=0;
    int bit = -2;
    char rx_val;
    std::string received;
  public:
    void reset_received() {
      this->received = "";
    }
    /**
     * 
     */
    void clock_cycle() {
      assert(this->top->clk == 0);
      this->step();
      this->last_value = this->top->tx_line;
      this->top->clk = 1;
      this->cycles++;
      this->step();
      if (this->last_value and !this->top->tx_line and this->bit == IDLE_BIT) {
        //std::cout << "Detected startbit at " << this->cycles << std::endl;
        this->bit = START_BIT;
        this->next_cycle = this->cycles + (BAUD_500000_CYLCES/2);
        this->rx_val = 0;
      }
      this->last_value = this->top->tx_line;
      if (this->cycles == this->next_cycle) {
          if (this->bit >= FIRST_BIT && this->bit <= LAST_BIT) {
            this->rx_val |= (this->top->tx_line << this->bit);
          }
          this->bit++;
          this->next_cycle = this->cycles + BAUD_500000_CYLCES;
      }
      if (this->bit==STOP_BIT) {
        this->next_cycle = 0;
        this->bit = IDLE_BIT;
        this->received += this->rx_val;
        //std::cout << "STOP BIT '" << this->rx_val << "' " << (int)(this->rx_val) << " " << std::endl;
      }
      this->top->clk = 0;
    }
};