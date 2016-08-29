#pragma once
#include "animation.h"
#include "leds.h"
#include "visualization.h"

namespace pixelmap {

class TriangleAnimation : public Animation {
  public:
    TriangleAnimation(Visualization* viz, LEDs& leds);
    ~TriangleAnimation();
    void init();
    void init(double scale, bool outer = false);
    void init_inner(double scale);
    void init_outer(double scale);
    void update();
    void draw(float interpolation);

  private:
    LEDs& leds_;
    Visualization* viz_;
    double* mapping_;
};

} // end namespace pixelmap
