#pragma once
#include "animation.h"
#include "leds.h"
#include "visualization.h"

namespace pixl {

class MarchAnimation : public Animation {
  public:
    MarchAnimation(Visualization* viz, LEDs& leds);
    void init();
    void update();
    void draw(float interpolation);

  private:
    LEDs& leds_;
    Visualization* viz_;
};

} // end namespace pixl
