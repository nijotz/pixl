#include "assert.h"
#include "leds.h"

namespace pixl {

LEDStrip::LEDStrip(int length)
  : length(length)
{
  // Initialze one array to hold LED data for all the strips
  leds = new CRGB[length];

  for (int i = 0; i < length; i++) {
    leds[i] = CRGB::Red;
  }
}

LEDStrip::~LEDStrip() {
  delete[] leds;
}

LEDs::LEDs(LEDStrip* strip, int start, int length, bool reverse) {
  LEDStrip* strips[] = {strip};
  int starts[] = {start};
  int lengths[] = {length};
  init(1, strips, starts, lengths, reverse);
}

LEDs::LEDs(int num_strips, LEDStrip* strips[], int* starts, int* lengths,
           bool reverse) {
  init(num_strips, strips, starts, lengths, reverse);
}

void LEDs::init(int num_strips, LEDStrip* strips[], int* starts, int* lengths,
                bool reverse) {
  strips_ = new LEDStrip*[num_strips];
  starts_ = new int[num_strips];
  lengths_ = new int[num_strips];
  num_strips_ = num_strips;
  length_ = 0;
  reverse_ = reverse;

  for (int i = 0; i < num_strips; i++) {
    LEDStrip* strip = strips[i];
    int start = starts[i];
    int length = lengths[i];
    assert(start >= 0 && start < strip->length);
    assert(start + length - 1 < strip->length);

    strips_[i] = strip;
    starts_[i] = start;
    lengths_[i] = length;

    length_ += length;
  }
}

LEDs::~LEDs() {
  delete[] strips_;
  delete[] starts_;
  delete[] lengths_;
}

CRGB& LEDs::operator[](int index) {
  assert(index >= 0 && index < length_);

  for (int i = 0; i < num_strips_; i++) {
    if (index < lengths_[i]) {
      if (reverse_) {
        index = starts_[i] + lengths_[i] - 1 - index;
      } else {
        index = starts_[i] + index;
      }
      return strips_[i]->leds[index];
    }

    index -= lengths_[i];
  }
}

void LEDs::fillSolid(CRGB color) {
  for (int i = 0; i < num_strips_; i++) {
    for (int j = starts_[i]; j < lengths_[i]; j++) {
      strips_[i]->leds[j] = color;
    }
  }
}

} // end namespace pixl
