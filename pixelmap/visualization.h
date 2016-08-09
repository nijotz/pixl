#pragma once
#include <FastLED.h>
#include "input.h"

namespace pixelmap {

/* A Visualization is an interface to an n-dimensional set of colors to be used
   in Animations.  Colors are stored as FastLED's CHSV class to make it easier
   for animations to map the colors to a set of LEDs */
class Visualization {
  public:
    Visualization(Input* input) : input(input) {};
    virtual void update()=0;
    CRGB viz[1];
    Input* input;
};

class HueVisualization : public Visualization {
  public:
    HueVisualization(Input* input);
    void update();
};

} // end namespace pixelmap
