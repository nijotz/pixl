#pragma once
#include "input.h"
#include <Audio.h>

namespace pixl {

class FFTInput : public Input {
  public:
    FFTInput(AudioAnalyzeFFT1024* fft, int start=0, int range=512);
    void update();
    float getInput(int index = 0);
  private:
    AudioAnalyzeFFT1024* fft_;
    uint16_t max_[512] = {0};
    float start_;
    float range_;
    float amp_;
    float freq_;
};

} // end namespace pixl
