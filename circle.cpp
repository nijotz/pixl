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

  float radius = (float)length / (2.0 * 3.1415);
  Log.Debug("Radius: %d (x1000)\n", (int)(radius * 1000.0));

  float viz_scaled = viz_->getSize() * scale;

  for (int i = 0; i < length; i++) {
    float radians = ((float)i / ((float)length - 1.0)) * (2.0 * 3.1415);
    float a = sin(radians) * radius;
    float b = cos(radians) * radius;
    float y = radius - b;
    float x = sqrt(a * a + y * y);
    float map = x / viz_scaled;
    mapping_[i] = x / viz_scaled;
    Log.Debug("Mapping of %d: %d (x1000)\n", i, (int)(map * 1000.0));
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

