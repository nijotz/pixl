#include "march.h"

using namespace pixl;

MarchAnimation::MarchAnimation(Visualization* viz, LEDs& leds)
  : leds_(leds),
    viz_(viz) {}

void MarchAnimation::init() {}
void MarchAnimation::update() {}

void MarchAnimation::draw(float interpolation) {
  for (int i = leds_.length() - 1; i > 0; i--) {
    leds_[i] = leds_[i - 1];
  }

  leds_[0] = viz_->getColorByIndex(0);
}
