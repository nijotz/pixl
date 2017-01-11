#pragma once
#include "leds.h"
#include "visualization.h"

namespace pixl {

/* An Animation takes a visualization and maps it to LED segments */
class Animation {
  public:
    virtual void init()=0;
    virtual void update()=0;
    virtual void draw(float interpolation)=0;
};

class ScaledAnimation : public Animation {
  public:
    ScaledAnimation(Visualization* viz, LEDs& leds);
    void init();
    void update();
    void draw(float interpolation);
    void upscale();
    void downscale();

  private:
    LEDs& leds_;
    Visualization* viz_;
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
};

} // end namespace pixl
