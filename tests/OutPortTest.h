#pragma once

#include <string>

#include "HardwareAddonTest.h"

template<class TOP>
class OutPortTest : public HardwareAddonTest {
  protected:
    TOP* top;
    int last_value = 0;
    std::string received;
  public:
    OutPortTest(TOP* top) : top(top) {
      this->received = "";
    }

    void reset_received() {
      this->received = "";
    }

    std::string get_received() {
      return this->received;
    }

    void clock_low() {
      clock_high();
    }

    void clock_high() {
      if (top->port != last_value) {
        this->received += (char)top->port;
      }
      last_value = top->port;
    }
};