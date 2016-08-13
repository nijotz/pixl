#pragma once
#include <FastLED.h>

namespace pixelmap {

/* LEDStrip represents a FastLED-compatible LED strip. */
class LEDStrip {
  public:
    LEDStrip(int length);
    ~LEDStrip();
    void AssignPin(int pin);
    int length;
    CRGB* leds;
};

/* LEDs represents a section of an LED strip. */
class LEDs {
  public:
    LEDs(LEDStrip& strip, int start, int length);
    CRGB& operator[](int index);
    int length() { return length_; }
    LEDStrip& strip_;
  private:
    int start_;
    int length_;
};

} // end namespace pixelmap
