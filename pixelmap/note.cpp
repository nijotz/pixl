#include "note.h"
#include <Logging.h>

using namespace pixelmap;

namespace pixelmap {

NoteInput::NoteInput() : value_(0) {
  Log.Info("Initializing Note input\n");
  AudioMemory(30);

  input_ = new AudioInputAnalog();
  tuner_ = new AudioTuner();
  patch_cord_ = new AudioConnection(*input_,  0, *tuner_, 0);

  // Initialize the yin algorithm's absolute threshold (0.15 is good)
  // and maximum cpu usage
  tuner_->initialize(0.15, 0.5);

  Log.Debug("Done initializing Note input\n");
  delay(100);
}

NoteInput::~NoteInput() {
  delete input_;
  delete tuner_;
  delete patch_cord_;
}

void NoteInput::update() {
  int base_a4 = 440;

  // read back fundamental frequency
  if (tuner_->available()) {
    float freq = tuner_->read();
    //frequency to midi conversion
    int note = round(12 * (log(freq / base_a4) / log(2)) + 69);
    float prob = tuner_->probability();
    Log.Debug("Note: %3.2f | Probability: %.2f\n", note, prob);
    value_ = (float)note / 100.0;
    value_ = min(1.0, value_);
    value_ = max(0.0, value_);
  }
}

float NoteInput::getInput() {
  return value_;
}

} // end namespace pixelmap
