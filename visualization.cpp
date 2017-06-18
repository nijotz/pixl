#include <assert.h>
#include "pixl.h"

using namespace pixl;

namespace pixl {

CRGB Visualization::getColorByRatio(float ratio) {
  assert(ratio >= 0);
  assert(ratio <= 1);
  float viz_index = (float)(size_ - 1.0) * ratio;
  return getColorByIndex(viz_index);
}

CRGB Visualization::getColorByIndex(float index) {
  assert(index >= 0);
  assert(index < size_);
  CRGB color1 = viz_[(int)floor(index)];
  CRGB color2 = viz_[(int)ceil(index)];
  color1 /= 2;
  color2 /= 2;
  return color1 + color2;
}

CRGB Visualization::getColorByIndex(int index) {
  assert(index >= 0);
  assert(index < size_);
  return viz_[index];
}

HueVisualization::HueVisualization(Input* input, int size)
  : Visualization(input, size) {}

void HueVisualization::update() {
  float val = input_->getInput();
  viz_[0] = CHSV(val * 255, 255, 255);
}

} // end namespace pixl
