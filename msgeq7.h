#pragma once
#include "input.h"

namespace pixl {

class MSGEQ7Input : public Input {
  public:
    MSGEQ7Input(int left_pin, int right_pin,
                int strobe_pin, int reset_pin,
                float min_amp, float max_amp,
                float min_freq = 0, float max_freq = 0,
                bool amplitude = true, bool mono = true);
    void update();
    float getInput(int index = 0);
  private:
    void updateAmplitude();
    void updateFrequency();
    int strobe_pin_;
    int reset_pin_;
    int left_pin_;
    int right_pin_;
    float min_amp_;
    float max_amp_;
    float min_freq_;
    float max_freq_;
    int amplitude_;
    int mono_;
    float amp_;
    float freq_;
};

} // end namespace pixl
