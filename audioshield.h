#pragma once
#include "input.h"
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

namespace pixl {

class AudioShieldInput : public Input {
  public:
    AudioShieldInput(AudioAnalyzePeak* peak, AudioAnalyzeNoteFrequency* note);
    void update();
    float getInput(int index = 0);
  private:
    AudioAnalyzePeak* peak_;
    AudioAnalyzeNoteFrequency* note_;
    float amp_;
    float freq_;
};

} // end namespace pixl
