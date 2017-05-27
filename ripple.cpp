#include "pixl.h"

using namespace pixl;

namespace pixl {

RippleVisualization::RippleVisualization(Input* input, int size, int smoothing, bool freq)
    : Visualization(input, size),
      smoothing_length_(smoothing),
      freq_(freq) {
  smoothing_amp_ = new float[smoothing_length_];
  smoothing_freq_ = new float[smoothing_length_];

  for (int i = 0; i < smoothing_length_; i++) {
    smoothing_amp_[i] = 0.0;
    smoothing_freq_[i] = 0.0;
  }
}

RippleVisualization::~RippleVisualization() {
  delete[] smoothing_amp_;
  delete[] smoothing_freq_;
}

template <typename Type>
Type smooth(Type* smoothing, int length, Type value) {
  Type sum = 0.0;
  for (int i = 0; i < length; i++) {
    sum += smoothing[i];
  }
  sum += value;
  value = sum / (float)(length + 1);

  PushQueue(smoothing, length, value);

  return value;
}

void RippleVisualization::update_amp() {
  float value = input_->getInput();

  value = smooth(smoothing_amp_, smoothing_length_, value);

  int hue = 192 + (-value * 192);
  int val = 255;
  if (value < 0.1) { val = 0; }
  CRGB color = CHSV(hue, 255, val);
  PushQueue(viz_, size_, color);
}

void RippleVisualization::update_freq() {
  float amp = input_->getInput(0);
  float freq = input_->getInput(1);

  amp = smooth(smoothing_amp_, smoothing_length_, amp);
  freq = smooth(smoothing_freq_, smoothing_length_, freq);

  int hue = 192 * freq;
  int val = 255.0 * amp;
  if (val < 10) { val = 0; }
  CRGB color = CHSV(hue, 255, val);
  PushQueue(viz_, size_, color);
}

void RippleVisualization::update() {
  if (freq_) {
    update_freq();
  } else {
    update_amp();
  }
}

} // end namespace pixl
