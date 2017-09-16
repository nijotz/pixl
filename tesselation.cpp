#include "tesselation.h"
#include <Logging.h>

using namespace pixl;

TesselationAnimation::TesselationAnimation(Visualization* viz, LEDs& leds)
    : leds_(leds),
      viz_(viz) {}

TesselationAnimation::~TesselationAnimation() {
  delete[] mapping_;
}

void TesselationAnimation::init(
    float rotate = 0.0, float scale = 1.0,
    float tx = 0.0, float ty = 0.0) {
  // https://i.imgur.com/CLxUSjP.jpg
  Log.Info("Setting up Tesselation animation\n");

  mapping_ = new float[64];

  int strip_leds = 15; // How many leds on the current strip
  int strip = 0;       // The current strip
  int strip_led = 0;   // The current led on the strip
  int strip_dir = 1;   // The direction we are moving along the strip

  for (int led = 0; led < 64; led++) {
    // Each strip is 6 squares further than the previous and each LED is on
    // average 1.5 squares further than the previous
    float x = (strip * 6) + (strip_led * 1.5);

    // Each led is on average 2.5 squares higher than the next one
    float y = (strip_led * 2.5);

    // Rotate point
    float c = cos(rotate);
    float s = sin(rotate);
    float xnew = x * c - y * s;
    float ynew = x * s - y * c;
    x = xnew;
    y = ynew;

    // The triangle is about 45 squares, scale to [0,1] for mapping
    x = x / 45.0;
    y = y / 45.0;

    // Scale coordinates
    x *= scale;
    y *= scale;

    // Translate point
    x += tx;
    y += ty;

    // Get the distance from the staring LED using pythag
    mapping_[led] = sqrt(pow(x, 2) + pow(y, 2));
    Log.Debug("Mapping %d: %d\n", led, int(mapping_[led] * 1000.0));

    // Move along the strip according to the current direction
    strip_led += strip_dir;

    // If we've gone beyond the strip
    if (strip_led == strip_leds || strip_led == -1) {
      strip++;         // continue to the next strip
      strip_leds -= 2; // LED strips decrease by two going right
      strip_dir *= -1; // Reverse the direction

      // Start at the top or bottom according to the direction of this strip
      if (strip_dir == -1) {
        strip_led = strip_leds - 1;
      } else {
        strip_led = 0;
      }
    }
  }

  Log.Info("Finished setting up Tesselation animation\n");
}

void TesselationAnimation::update() {}

void TesselationAnimation::draw(float interpolation) {
  for (int i = 0; i < 64; i++) {
    leds_[i] = viz_->getColorByRatio(mapping_[i]);
  }
}

