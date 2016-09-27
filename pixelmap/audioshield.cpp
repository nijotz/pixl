#include "audioshield.h"
#include <Logging.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

using namespace pixelmap;

namespace pixelmap {

AudioShieldInput::AudioShieldInput(AudioAnalyzePeak* peak) : peak_(peak), value_(0.2) {}

void AudioShieldInput::update() {
  if (peak_->available()) {
    value_ = peak_->read();
  }
}

float AudioShieldInput::getInput() {
  return value_;
}

} // end namespace pixelmap
