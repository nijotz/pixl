#include "pixelmap.h"

using namespace pixelmap;

namespace pixelmap {

RippleVisualization::RippleVisualization(Input* input, int size, int smoothing, bool freq)
    : Visualization(input, size),
      smoothing_length_(smoothing),
      freq_(freq) {
  smoothing_ = new double[smoothing_length_];

  for (int i = 0; i < smoothing_length_; i++) {
    smoothing_[i] = 0.0;
  }
}


RippleVisualization::~RippleVisualization() {
  delete[] smoothing_;
}

void RippleVisualization::update_amp() {
  double value = input->getInput();

  double sum = 0.0;
  for (int i = 0; i < smoothing_length_; i++) {
    sum += smoothing_[i];
  }
  sum += value;
  value = sum / (double)(smoothing_length_ + 1);

  PushQueue(smoothing_, smoothing_length_, value);

  int hue = 192 + (-value * 192);
  int val = 255;
  if (value < 0.001) { val = 0; }
  CRGB color = CHSV(hue, 255, val);
  PushQueue(viz, size_, color);
}

void RippleVisualization::update_freq() {
  float amp = input->getInput(0);
  float freq = input->getInput(1);

  int hue = 192 * freq;
  int val = 255.0 * amp;
  if (val < 10) { val = 0; }
  CRGB color = CHSV(hue, 255, val);
  PushQueue(viz, size_, color);
}

void RippleVisualization::update() {
  if (freq_) {
    update_freq();
  } else {
    update_amp();
  }
}

} // end namespace pixelmap
