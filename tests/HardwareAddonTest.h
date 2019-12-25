#pragma once

class HardwareAddonTest {
  public:
    virtual void clock_high() = 0;
    virtual void clock_low() = 0;
};
