#include "animation.h"

using namespace pixelmap;

PassThroughAnimation::PassThroughAnimation(Visualization* viz, LEDs& leds)
  : leds_(leds),
    viz_(viz) {}

void PassThroughAnimation::init() {}
void PassThroughAnimation::update() {}

void PassThroughAnimation::draw(float interpolation) {
  for(int i = 0; i < leds_.length(); i++) {
    leds_[i] = viz_->viz[i];
  }
}
