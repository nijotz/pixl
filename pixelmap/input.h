#pragma once
#include <Arduino.h>

namespace pixelmap {

/* An Input is an interface for Visualizations to use. getInput() returns a
   float between 0 and 1. */
class Input {
  public:
    Input(){};
    void update() {};
    virtual float getInput()=0;
};

class NullInput : public Input {
  public:
    NullInput(){};
    void update() {};
    float getInput() { return 0.0; }
};

class FullInput : public Input {
  public:
    FullInput(){};
    void update() {};
    float getInput() { return 1.0; }
};

class ConstantInput : public Input {
  public:
    ConstantInput(float value) : value_(value) {};
    void update() {};
    float getInput() { return value_; }
  private:
    float value_;
};

class WaveInput : public Input {
  public:
    WaveInput(){};
    void update() {};
    float getInput() { return (sin(2.0 * 3.14 * (millis() % 333) / 333.0) + 1.0) / 2.0; }
};

class RandomInput : public Input {
  public:
    RandomInput(){};
    void update() {};
    float getInput() { return (float) rand() / (float) RAND_MAX; }
};

// This will return 1.0 only once per press by keeping track of when the button
// is released
class ButtonInput : public Input {
  public:
    ButtonInput(int pin) : pin_(pin), released_(true) {};
    void update() {};
    float getInput() {
      if (digitalRead(pin_) == HIGH) {
        if (released_ == true) {
          released_ = false;
          return 1.0;
        }
      } else if (released_ == false) {
        released_ = true;
      }
      return 0.0;
    }
  private:
    int pin_;
    bool released_;
};

} // end namespace pixelmap
