#include "msgeq7.h"
#include <Logging.h>

using namespace pixelmap;

namespace pixelmap {

MSGEQ7Input::MSGEQ7Input(int left_pin, int right_pin,
                       int strobe_pin, int reset_pin,
                       int max_amplitude, int min_amplitude)
  : strobe_pin_(strobe_pin), reset_pin_(reset_pin),
    left_pin_(left_pin), right_pin_(right_pin),
    max_amplitude_(max_amplitude), min_amplitude_(min_amplitude)
{
  pinMode(left_pin_, INPUT);
  pinMode(right_pin_, INPUT);
  pinMode(strobe_pin_, OUTPUT);
  pinMode(reset_pin_, OUTPUT);

  digitalWrite(reset_pin_, LOW);
  digitalWrite(strobe_pin_, HIGH);
}

void MSGEQ7Input::update() {
  // Reset EQ7 chip
  digitalWrite(reset_pin_, HIGH);
  digitalWrite(reset_pin_, LOW);

  double total = 0.0;

  // Get readings from chip, sum freq values
  for (int i = 0; i < 4; i++) {
    digitalWrite(strobe_pin_, LOW);
    delayMicroseconds(30); // to allow the output to settle

    total += analogRead(left_pin_);
    total += analogRead(right_pin_);

    // strobe to the next frequency
    digitalWrite(strobe_pin_, HIGH);
  }

  value_ = total / 4.0;

  // Subtract min amplitude so lowest value is 0
  value_ -= min_amplitude_;
  double max = (max_amplitude_ - min_amplitude_);
  value_ = (value_ / max);

  value_ = min(1.0, value_);
  value_ = max(0.0, value_);
}

float MSGEQ7Input::getInput() {
  return value_;
}

} // end namespace pixelmap
