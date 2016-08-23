#pragma once
#include "input.h"

namespace pixelmap {

class MSGEQ7Input : public Input {
  public:
    MSGEQ7Input(int left_pin, int right_pin, int strobe_pin, int reset_pin, int max_amplitude, int min_amplitude);
    void update();
    float getInput();
  private:
    int strobe_pin_;
    int reset_pin_;
    int left_pin_;
    int right_pin_;
    int max_amplitude_;
    int min_amplitude_;
    float value_;
};

} // end namespace pixelmap
