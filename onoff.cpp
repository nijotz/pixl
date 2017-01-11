#include "pixl.h"

using namespace pixl;

namespace pixl {

OnOffVisualization::OnOffVisualization(Input* input, int smoothing)
    : Visualization(input, 1),
      smoothing_length_(smoothing) {
  smoothing_ = new double[smoothing_length_];

  for (int i = 0; i < smoothing_length_; i++) {
    smoothing_[i] = 0.0;
  }
}

OnOffVisualization::~OnOffVisualization() {
  delete[] smoothing_;
}

void OnOffVisualization::update() {
  double value = input_->getInput();

  double sum = 0.0;
  for (int i = 0; i < smoothing_length_; i++) {
    sum += smoothing_[i];
  }
  sum += value;
  value = sum / (double)(smoothing_length_ + 1);

  PushQueue(smoothing_, smoothing_length_, value);

  CRGB color = CRGB::Blue;
  color.fadeToBlackBy(255 - (255 * value));
  viz_[0] = color;
}

} // end namespace pixl
