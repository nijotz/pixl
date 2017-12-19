#if defined(__MK20DX256__) || defined(__MK20DX128__) || defined(__MK66FX1M0__)

#include "fft.h"
#include <Logging.h>
#include <Audio.h>

using namespace pixl;

namespace pixl {

FFTInput::FFTInput(AudioAnalyzeFFT1024* fft, int start, int range)
  : fft_(fft), amp_(0.0), freq_(0.0),
    start_(start), range_(range) {}

void FFTInput::update() {
  if (fft_->available()) {
    float max_delta = 0.0;
    int max_value = 0;
    int max_index = 0;

    for (int i = start_; i < range_; i++) {
      int value = fft_->output[i];
      float delta = (float)value / (float)max_[i];

      // Decay max
      max_[i] = max_[i] * 0.993;

      // Track max
      if (value > max_[i]) {
        max_[i] = value;
      }

      // Find max amplitude
      if (value > max_value) {
        max_value = value;
      }

      // Track the freq that changed the most
      if (delta > max_delta) {
        max_delta = delta;
        max_index = i;
      }
    }

    Log.Debug("max index: %d\n", max_index);
    Log.Debug("max value: %d\n", max_value);

    freq_ = (float)max_index / (range_ - 1);
    freq_ = min(1.0, freq_);

    amp_ = (float)max_value / 6000.0;
    amp_ = min(1.0, amp_);
  }
}

float FFTInput::getInput(int index) {
  if (index == 0) {
    return amp_;
  } else if (index == 1) {
    return freq_;
  } else {
    return 0.0;
  }
}

} // end namespace pixl

#endif
