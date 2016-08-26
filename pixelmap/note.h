#pragma once
#include "input.h"
#include <AudioTuner.h>
#include <Audio.h>

namespace pixelmap {

class NoteInput : public Input {
  public:
    NoteInput();
    ~NoteInput();
    void update();
    float getInput();
    void init();
  private:
    AudioInputAnalog* input_;
    AudioTuner* tuner_;
    AudioConnection* patch_cord_;
    float value_;
};

} // end namespace pixelmap
