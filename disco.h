#pragma once
#include "animation.h"
#include "leds.h"

namespace pixl {

class DiscoAnimation : public Animation {
  public:
    DiscoAnimation(Visualization* viz, LEDs& leds);
    void init();
    void update();
    void draw(float interpolation);
    void drawRainbowDashes(
      uint8_t startpos, uint16_t lastpos, uint8_t period, uint8_t width,
      uint8_t huestart, uint8_t huedelta, uint8_t saturation, uint8_t value);
    void discoWorker(
        uint8_t dashperiod, uint8_t dashwidth, int8_t  dashmotionspeed,
        uint8_t stroberepeats,
        uint8_t huedelta);

  private:
    LEDs& leds_;
    Visualization* viz_;
    float wave_;
    float amplitude_;
};

} // end namespace pixl
