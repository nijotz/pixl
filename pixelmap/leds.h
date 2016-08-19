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
    LEDs(LEDStrip* strip, int start, int length);
    LEDs(int num_strips, LEDStrip* strips[], int* starts, int* lengths);
    ~LEDs();
    CRGB& operator[](int index);
    int length() { return length_; }
    void fillSolid(CRGB color);
  private:
    void init(int num_strips, LEDStrip* strips[], int* starts, int* lengths);
    int length_;
    int num_strips_;
    LEDStrip** strips_;
    int* starts_;
    int* lengths_;
};

} // end namespace pixelmap
