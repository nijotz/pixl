#include "pixelmap.h"

using namespace pixelmap;

namespace pixelmap {

template <typename Type>
Type PushQueue(Type* array, int length, Type value) {
  Type rtrn = array[length - 1];
  for (int i = (length - 1); i > 0; i--) {
    array[i] = array[i - 1];
  }
  array[0] = value;
  return rtrn;
}

RippleVisualization::RippleVisualization(Input* input, int size)
  : Visualization(input, size)
  {
    for (int i = 0; i < smoothing_length_; i++) {
      smoothing_[i] = 0.0;
    }
  }

void RippleVisualization::update() {
  float value = input->getInput();

  Log.Debug("value: %d", (int)(value * 255.0));

  float sum = 0.0;
  for (int i = 0; i < smoothing_length_; i++) {
    sum += smoothing_[i];
  }
  sum += value;
  value = sum / (float)(smoothing_length_ + 1);

  PushQueue(smoothing_, smoothing_length_, value);

  int hue = (int)(value * 255.0);
  CRGB color = CHSV(hue, 255, 255);
  PushQueue(viz, size_, color);
}

} // end namespace pixelmap
