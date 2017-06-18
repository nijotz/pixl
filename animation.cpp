#include <Assert.h>
#include "animation.h"

using namespace pixl;

PassThroughAnimation::PassThroughAnimation(Visualization* viz, LEDs& leds)
  : leds_(leds),
    viz_(viz) {}

void PassThroughAnimation::init() {}
void PassThroughAnimation::update() {}

void PassThroughAnimation::draw(float interpolation) {
  for(int i = 0; i < leds_.length(); i++) {
    // TODO: tried by ratio to support different led length and viz length but
    // got rounding errors and color spilled over to two pixels
    leds_[i] = viz_->getColorByIndex(i);
  }
}

ScaledAnimation::ScaledAnimation(Visualization* viz, LEDs& leds)
  : leds_(leds),
    viz_(viz) {}

void ScaledAnimation::init() {}
void ScaledAnimation::update() {}

// Linear upscale
void ScaledAnimation::upscale() {
  int led_length = leds_.length();
  int viz_length = viz_->getSize();

  // Subtract one from the lengths to get scale because of fencepost problem
  //
  // Example: If there are 5 LEDs and 3 viz colors, the first color will be
  // at 0, the last color at 4, and the other should be at 2.  0, 2, 4.
  // Every 2 LEDs. (5 - 1) / (3 - 1) = 2
  // LEDs:   | | | | |
  // Viz:    +   +   +
  //
  // Example: If there are 10 LEDs and 4 viz colors, the first color will be
  // at 0, the last color at 9, leaving the other two at 3 and 6. 0, 3, 6, 9.
  // Every 3 LEDs. (10 - 1) / (4 - 1) = 3
  // LEDs:   | | | | | | | | | |
  // Viz:    +     +     +     +
  float scale = ((float)led_length - 1.0) / ((float)viz_length - 1.0);

  // Set up start and end fence posts
  leds_[0] = viz_->getColorByIndex(0);
  leds_[led_length - 1] = viz_->getColorByIndex(viz_length - 1);

  // Fill in the middle
  for (int i = 1; i < led_length - 1; i++) {
    // Where along the viz array this LED should get its color (decimal)
    float index = (float)i / scale;

    // Get the indices of the two viz colors this LED will blend
    int index1 = (int)floor(index);
    int index2 = index1 + 1;

    // Get the two viz colors this LED is between
    CRGB color1 = viz_->getColorByIndex(index1);
    CRGB color2 = viz_->getColorByIndex(index2);

    // Find how much of each viz color this LED should get
    int ratio1 = 1.0 - fmod(index, 1.0);
    int ratio2 = 1.0 - ratio1;

    // Blend the two colors this LED is between
    CRGB color = CRGB(
      (color1.r * ratio1) + (color2.r * ratio2),
      (color1.g * ratio1) + (color2.g * ratio2),
      (color1.b * ratio1) + (color2.b * ratio2)
    );

    leds_[i] = color;
  }
}

// Example: If there are 5 LEDs and 3 viz colors, the first color will be
// at 0, the last color at 4, and the other should be at 2.  0, 2, 4.
// Every 2 LEDs. (5 - 1) / (3 - 1) = 2
// LEDs:   | | | | |
// Viz:    +   +   +
//
// Example: If there are 10 LEDs and 4 viz colors, the first color will be
// at 0, the last color at 9, leaving the other two at 3 and 6. 0, 3, 6, 9.
// Every 3 LEDs. (10 - 1) / (4 - 1) = 3
// LEDs:   | | | | | | | | | |
// Viz:    +     +     +     +
void ScaledAnimation::downscale() {
  int led_length = leds_.length();
  int viz_length = viz_->getSize();

  float scale = ((float)viz_length - 1) / ((float)led_length - 1);

  for(int i = 0; i < led_length; i++) {
    leds_[i] = viz_->getColorByIndex((int)(i * scale));
  }
}

void ScaledAnimation::draw(float interpolation) {
  int led_length = leds_.length();
  int viz_length = viz_->getSize();

  // Upscale
  if (led_length > viz_length) {
    upscale();
  }

  // Downscale
  if (led_length < viz_length) {
    downscale();
  }

  // Pass through
  for(int i = 0; i < led_length; i++) {
    leds_[i] = viz_->getColorByIndex(i);
  }
}
