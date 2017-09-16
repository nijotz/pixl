#pragma once
#include "animation.h"
#include "leds.h"
#include "visualization.h"

namespace pixl {

class TesselationAnimation : public Animation {
  public:
    TesselationAnimation(Visualization* viz, LEDs& leds);
    ~TesselationAnimation();
    void init() {};
    void init(float rotate = 0.0, float scale = 1.0, float tx = 0.0, float ty = 0.0);
    void update();
    void draw(float interpolation);

  private:
    LEDs& leds_;
    Visualization* viz_;
    float* mapping_;
};

} // end namespace pixl
