// MirroringSample - see https://github.com/FastLED/FastLED/wiki/Multiple-Controller-Examples for more info on
// using multiple controllers.  In this example, we're going to set up four NEOPIXEL strips on four
// different pins, and show the same thing on all four of them, a simple bouncing dot/cyclon type pattern

#include <FastLED.h>

#define NUM_LEDS_PER_STRIP 60
CRGB leds[NUM_LEDS_PER_STRIP];

// For mirroring strips, all the "special" stuff happens just in setup.  We
// just addLeds multiple times, once for each strip
void setup() {
  // tell FastLED there's 60 NEOPIXEL leds on output 1
  FastLED.addLeds<NEOPIXEL, 2>(leds, NUM_LEDS_PER_STRIP); // or pin 6

  // tell FastLED there's 60 NEOPIXEL leds on output 2
  FastLED.addLeds<NEOPIXEL, 14>(leds, NUM_LEDS_PER_STRIP); // or pin 20

  // tell FastLED there's 60 NEOPIXEL leds on output 3
  FastLED.addLeds<NEOPIXEL, 7>(leds, NUM_LEDS_PER_STRIP); // or pin 21

  // tell FastLED there's 60 NEOPIXEL leds on output 4
  FastLED.addLeds<NEOPIXEL, 8>(leds, NUM_LEDS_PER_STRIP); // or pin 5
}

void loop() {
  for(int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
    // set our current dot to red
    leds[i] = CRGB::Red;
    FastLED.show();
    // clear our current dot before we move on
    leds[i] = CRGB::Black;
    delay(100);
  }

  for(int i = NUM_LEDS_PER_STRIP-1; i >= 0; i--) {
    // set our current dot to red
    leds[i] = CRGB::Red;
    FastLED.show();
    // clear our current dot before we move on
    leds[i] = CRGB::Black;
    delay(100);
  }
}
