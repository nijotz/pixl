#include <assert.h>
#include "pixelmap.h"

using namespace pixelmap;

namespace pixelmap {

CRGB Visualization::getColorByRatio(float ratio) {
  assert(ratio >= 0);
  assert(ratio <= 1);
  return viz[(int)((size_ - 1) * ratio)];
}

CRGB Visualization::getColorByIndex(float index) {
  assert(index >= 0);
  assert(index < size_);
  return viz[(int)floor(index)];
}

CRGB Visualization::getColorByIndex(int index) {
  assert(index >= 0);
  assert(index < size_);
  return viz[index];
}

HueVisualization::HueVisualization(Input* input, int size)
  : Visualization(input, size) {}

void HueVisualization::update() {
  float val = input->getInput();
  viz[0] = CHSV(val * 255, 255, 255);
}

} // end namespace pixelmap
