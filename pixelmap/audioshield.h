#pragma once
#include "input.h"
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

namespace pixelmap {

class AudioShieldInput : public Input {
  public:
    AudioShieldInput(AudioAnalyzePeak* peak);
    void update();
    float getInput();
  private:
    AudioAnalyzePeak* peak_;
    float value_;
};

} // end namespace pixelmap
