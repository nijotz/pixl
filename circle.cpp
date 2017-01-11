#include "circle.h"
#include <Logging.h>

using namespace pixl;

CircleAnimation::CircleAnimation(Visualization* viz, LEDs** leds, int num_leds)
    : leds_(leds),
      num_leds_(num_leds),
      viz_(viz) {}

CircleAnimation::~CircleAnimation() {
  delete[] mapping_;
}

void CircleAnimation::init() {}
void CircleAnimation::init(float scale) {
  Log.Info("Setting up Circle animation\n");

  int length = leds_[0]->length();
  mapping_ = new float[length];

  int radius = length / 2 * 3.1415;
  for (int i = 0; i < length; i++) {
    float radians = (float)(i / (length - 1)) * (2.0 * 3.1415);
    float a = sin(radians) * radius;
    float b = cos(radians) * radius;
    float y = radius - b;
    float x = sqrt(a * a + y * y);
    mapping_[i] = x;
  }

  Log.Info("Finished setting up Circle animation\n");
}

void CircleAnimation::update() {}

void CircleAnimation::draw(float interpolation) {
  for (int i = 0; i < num_leds_; i++) {
    LEDs* leds = leds_[i];
    for (int j = 0; j < leds->length(); j++) {
      (*leds)[j] = viz_->getColorByRatio(mapping_[j]);
    }
  }
}

