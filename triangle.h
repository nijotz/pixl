#pragma once
#include "animation.h"
#include "leds.h"
#include "visualization.h"

namespace pixl {

class TriangleAnimation : public Animation {
  public:
    TriangleAnimation(Visualization* viz, LEDs* leds[], int num_leds);
    ~TriangleAnimation();
    void init();
    void init(float scale, bool outer = false);
    void init_inner(float scale);
    void init_outer(float scale);
    void update();
    void draw(float interpolation);

  private:
    LEDs** leds_;
    int num_leds_;
    Visualization* viz_;
    float* mapping_;
};

} // end namespace pixl
