#include "audioshield.h"
#include <Logging.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

using namespace pixelmap;

namespace pixelmap {

AudioShieldInput::AudioShieldInput(AudioAnalyzePeak* peak, AudioAnalyzeNoteFrequency* note)
  : peak_(peak), note_(note), amp_(0.0), freq_(0.0) {}

void AudioShieldInput::update() {
  if (peak_->available()) {
    amp_ = peak_->read();
  }
  if (note_->available()) {
    float nt = note_->read();
    Serial.printf("Note: %3.2f\n", nt);
    freq_ = nt / 1000.0;
  }
}

float AudioShieldInput::getInput(int index) {
  if (index == 0) {
    return amp_;
  } else if (index == 1) {
    return freq_;
  } else {
    return 0.0;
  }
}

} // end namespace pixelmap
