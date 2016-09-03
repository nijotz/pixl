#include "pixelmap.h"

using namespace pixelmap;

namespace pixelmap {

RippleVisualization::RippleVisualization(Input* input, int size, int smoothing)
    : Visualization(input, size),
      smoothing_length_(smoothing) {
  smoothing_ = new double[smoothing_length_];

  for (int i = 0; i < smoothing_length_; i++) {
    smoothing_[i] = 0.0;
  }
}


RippleVisualization::~RippleVisualization() {
  delete[] smoothing_;
}

void RippleVisualization::update() {
  float amp = input->getInput();
  float freq = input->getInput2();

  int hue = 192 * freq;
  int val = 255.0 * amp;
  if (val < 10) { val = 0; }
  CRGB color = CHSV(hue, 255, val);
  PushQueue(viz, size_, color);
}

} // end namespace pixelmap
