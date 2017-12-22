#pragma once
#include "animation.h"
#include "leds.h"
#include "visualization.h"

namespace pixl {

class SpokesAnimation : public Animation {
  public:
    SpokesAnimation(Visualization* viz, LEDs& leds);
    ~SpokesAnimation();
    void init();
    void init(float scale);
    void update();
    void draw(float interpolation);
    void setVisualization(Visualization* viz);

  private:
    LEDs& leds_;
    Visualization* viz_;
    double* mapping_;
};

} // end namespace pixl
