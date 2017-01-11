#include "msgeq7.h"
#include <Logging.h>

using namespace pixl;

namespace pixl {

MSGEQ7Input::MSGEQ7Input(int left_pin, int right_pin,
                         int strobe_pin, int reset_pin,
                         float min_amp, float max_amp,
                         float min_freq, float max_freq,
                         bool amplitude, bool mono)
    : left_pin_(left_pin), right_pin_(right_pin),
      strobe_pin_(strobe_pin), reset_pin_(reset_pin),
      min_amp_(min_amp), max_amp_(max_amp),
      min_freq_(min_freq), max_freq_(max_freq),
      amplitude_(amplitude), mono_(mono) {
  pinMode(left_pin_, INPUT);
  pinMode(right_pin_, INPUT);
  pinMode(strobe_pin_, OUTPUT);
  pinMode(reset_pin_, OUTPUT);

  digitalWrite(reset_pin_, LOW);
  digitalWrite(strobe_pin_, HIGH);
}

float scaleValue(float value, float min, float max) {
  // Subtract min so lowest value is 0
  value -= min;
  max = (max - min);
  value = (value / max);

  value = min(1.0, value);
  value = max(0.0, value);

  return value;
}

void MSGEQ7Input::update() {
  if (amplitude_) {
    updateAmplitude();
  } else {
    updateFrequency();
    freq_ = scaleValue(freq_, min_freq_, max_freq_);
  }
  amp_ = scaleValue(amp_, min_amp_, max_amp_);
}

void MSGEQ7Input::updateAmplitude() {
  // Reset EQ7 chip
  digitalWrite(reset_pin_, HIGH);
  digitalWrite(reset_pin_, LOW);

  float total = 0.0;

  // Get readings from chip, sum freq values
  for (int i = 0; i < 7; i++) {
    digitalWrite(strobe_pin_, LOW);
    delayMicroseconds(30); // to allow the output to settle

    int left  = analogRead(left_pin_);
    int right = analogRead(right_pin_);
    if (mono_) {
      left = right;
    }

    total += left;
    total += right;

    // strobe to the next frequency
    digitalWrite(strobe_pin_, HIGH);
  }

  float avg = total / 14.0;
  amp_ = avg;
}

// This assumes that one instrument is playing one note.  To identify the
// frequency, it uses the frequency response chart in the documentation and
// assumes that there will be a rise in amplitude as the target frequency is
// approached and then a decent.
// https://www.sparkfun.com/datasheets/Components/General/MSGEQ7.pdf
// Complete shot on the dark on the start and end frequencies of each channel.
// As the graph in the documentation is inaccurate and hints at inaccuracies in
// trying to find the frequency using this method.  Also, there seems to be a
// curve to the peak and I'm just using linear interpolation.
//
// THIS IN NO WAY GUARANTEES ANY ACCURACY IN THE FREQUENCY RETURNED!
//
// Just an attempt to see what this chip spits out when I play a midi keyboard
// plugged into an AudioLux One while backstage at a deaf theatre production of
// The Wizard of Oz, putting something together for a presentation that takes
// place in 3 hours.  Definitely off the beaten path here.  On the beating
// path? #fmuf Which means I'm on the Right Path. :blessed: :smiling_imp:
void MSGEQ7Input::updateFrequency() {
  // Reset EQ7 chip
  digitalWrite(reset_pin_, HIGH);
  digitalWrite(reset_pin_, LOW);

  // Get readings from chip
  float channel_values[7] = {0};
  float max_value = 0.0;  // to be used below for relative scaling

  for (int i = 0; i < 7; i++) {
    digitalWrite(strobe_pin_, LOW);
    delayMicroseconds(30); // to allow the output to settle

    // Get this channel's value on a scale from 0 to 1
    float left  = analogRead(left_pin_)  / 1023.0;
    float right = analogRead(right_pin_) / 1023.0;
    if (mono_) {
      left = right;
    }
    float avg = (left + right) / 2.0;
    channel_values[i] = avg;

    // TODO: I hate working on a really tight timeline, makes me do stupid shit
    max_value = max(max_value, avg);

    // strobe to the next frequency
    digitalWrite(strobe_pin_, HIGH);
  }

  // When going left to right on the frequency response graphs of the lowest to
  // highest frequencies, assume the signal will rise on the descending side of
  // the frequency response graphs until it peaks, and then drops on the
  // ascending side of the curve.
  bool ascending = false;

  bool channel_ascending[7];

  // Determine which frequencies are on the descending side
  for (int i = 0; i < 7; i++) {
    if (// we've already switched to the ascending side
        ascending == true ||
        // we've reached the last frequency
        i == 6 ||
        // the next frequency is lower than this one
        channel_values[i + 1] < channel_values[i]) {
      // this channel is ascending
      ascending = true;
    }

    channel_ascending[i] = ascending;
  }

  // The start, peak, and end frequencies for each channel
  int freq_start[] = {  0,  60,  125,  300,  600,  1250,  2500};
  int freq_peak[]  = { 63, 160,  400, 1000, 2500,  6250, 16000};
  int freq_end[]   = {200, 400, 1000, 2500, 6250, 16000, 32000};

  // Approximate frequency
  float total = 0.0;
  for (int i = 0; i < 7; i++) {
    float channel_frequency = 0.0;

    // Scale this down to be between 0 and 1 where 1 is the maximum value
    float relative_scale = channel_values[i] / max_value;

    // ascending side
    if (channel_ascending[i]) {
      channel_frequency = (freq_peak[i] - freq_start[i]) * relative_scale;

    // descending side
    } else {
      channel_frequency = freq_end[i] - ((freq_end[i] - freq_start[i]) * relative_scale);
    }

    total += channel_frequency;
  }

  float freq = total / 7.0;
  freq_ = freq;
  Log.Debug("Detected Frequency: %d\n", (int)freq);
  amp_ = max_value * 1023.0;
}

float MSGEQ7Input::getInput(int index) {
  if (index == 0) {
    return amp_;
  } else if (index == 1) {
    return freq_;
  } else {
    return 0.0;
  }
}

} // end namespace pixl
