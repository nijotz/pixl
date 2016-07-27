#include "march.h"

using namespace pixelmap;

MarchAnimation::MarchAnimation(Visualization* viz, LEDs& leds)
  : leds_(leds),
    viz_(viz),
    wave_(0),
    amplitude_(0.0) {}

void MarchAnimation::init() {}
void MarchAnimation::update() {
  for(int i = leds_.length() - 1; i > 0; i--) {
    leds_[i] = leds_[i - 1];
  }

  leds_[0] = viz_->viz[0];
}

void MarchAnimation::draw(float interpolation) { }
