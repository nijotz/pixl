#pragma once
#include "leds.h"
#include "visualization.h"

namespace pixelmap {

/* An Animation takes a visualization and maps it to LED segments */
class Animation {
  public:
    virtual void init()=0;
    virtual void update()=0;
    virtual void draw(float interpolation)=0;
};

class PassThroughAnimation : public Animation {
  public:
    PassThroughAnimation(Visualization* viz, LEDs& leds);
    void init();
    void update();
    void draw(float interpolation);

  private:
    LEDs& leds_;
    Visualization* viz_;
    float wave_;
    float amplitude_;
};

} // end namespace pixelmap
