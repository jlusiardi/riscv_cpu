#pragma once

#include <string>

#include "HardwareAddonTest.h"

#define IDLE_BIT -2
#define START_BIT -1
#define FIRST_BIT 0
#define LAST_BIT 7
#define STOP_BIT 8
#define BAUD_500000_CYLCES 101

template<class TOP>
class SerialTest : public HardwareAddonTest {
  protected:
    TOP* top;
    int last_value = -1;
    int next_cycle=0;
    int bit = -2;
    char rx_val;
    std::string received;
    int cycles;
  public:
    SerialTest(TOP* top) : top(top) {}

    void reset_received() {
      this->received = "";
    }

    std::string get_received() {
      return this->received;
    }

    void clock_low() {
      //std::cout << "low" << this->top << std::endl;
      this->last_value = this->top->tx_line;
    }

    void clock_high() {
      //std::cout << "high" << std::endl;
      this->cycles++;
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
    }
};