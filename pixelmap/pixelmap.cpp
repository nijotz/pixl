#include <assert.h>
#include "pixelmap.h"
#include <FastLED.h>
#include <Logging.h>

#define LOGLEVEL LOG_LEVEL_DEBUG


namespace pixelmap {

/*
 *  LEDStrip
 */

LEDStrip::LEDStrip(int length)
  : length(length)
{

  // Initialze one array to hold LED data for all the strips
  leds = new CRGB[length];

  for (int i = 0; i < length; i++) {
    leds[i] = CRGB::Red;
  }
}

/*
 *  LEDs
 */

LEDs::LEDs(LEDStrip& strip, int start, int length)
  : length_(length),
    start_(start),
    strip_(strip)
{
  assert(start_ >= 0 && start_ < strip_.length);
  assert(start_ + length_ - 1 < strip_.length);
}

CRGB& LEDs::operator[](int index) {
  assert(index >= 0 && index < length_);
  return strip_.leds[index + start_];
}

} // end namespace pixelmap
