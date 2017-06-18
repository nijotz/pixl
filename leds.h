#pragma once
#include <FastLED.h>

namespace pixl {

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
    LEDs(LEDStrip* strip, int start, int length, bool reverse = false);
    LEDs(int num_strips, LEDStrip* strips[], int* starts, int* lengths,
         bool reverse = false);
    ~LEDs();
    CRGB& operator[](int index);
    int length() { return length_; }
    void fillSolid(CRGB color);
  private:
    void init(int num_strips, LEDStrip* strips[], int* starts, int* lengths,
              bool reverse);
    int length_;
    int num_strips_;
    LEDStrip** strips_;
    int* starts_;
    int* lengths_;
    int reverse_;
};

} // end namespace pixl
