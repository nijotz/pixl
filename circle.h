#pragma once
#include "animation.h"
#include "leds.h"
#include "visualization.h"

namespace pixl {

class CircleAnimation : public Animation {
  public:
    CircleAnimation(Visualization* viz, LEDs* leds[], int num_leds);
    ~CircleAnimation();
    void init();
    void init(float scale);
    void update();
    void draw(float interpolation);

  private:
    LEDs** leds_;
    int num_leds_;
    Visualization* viz_;
    float* mapping_;
};

} // end namespace pixl
