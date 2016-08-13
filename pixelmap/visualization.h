#pragma once
#include <FastLED.h>
#include "input.h"

namespace pixelmap {

/* A Visualization is an interface to an n-dimensional set of colors to be used
   in Animations.  Colors are stored as FastLED's CHSV class to make it easier
   for animations to map the colors to a set of LEDs */
class Visualization {
  public:
    Visualization(Input* input, int size = 1)
      : input(input),
        size_(size)
    {
      viz = new CRGB[size];
    };
    ~Visualization() { delete[] viz; }

    virtual void update()=0;
    int getSize() { return size_; };
    CRGB* viz;
    Input* input;
  protected:
    int size_;
};

class HueVisualization : public Visualization {
  public:
    HueVisualization(Input* input, int size = 1);
    void update();
};

} // end namespace pixelmap
