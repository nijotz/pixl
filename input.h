#pragma once
#include <Arduino.h>

namespace pixl {

/* An Input is an interface for Visualizations to use. getInput() returns a
   float between 0 and 1. */
class Input {
  public:
    Input(){};
    virtual void update()=0;
    virtual float getInput(int index = 0)=0;
};

class NullInput : public Input {
  public:
    NullInput(){};
    void update() {};
    float getInput(int index = 0) { return 0.0; }
};

class FullInput : public Input {
  public:
    FullInput(){};
    void update() {};
    float getInput(int index = 0) { return 1.0; }
};

class ConstantInput : public Input {
  public:
    ConstantInput(float value) : value_(value) {};
    void update() {};
    float getInput(int index = 0) { return value_; }
  private:
    float value_;
};

class WaveInput : public Input {
  public:
    WaveInput(float seconds = 1.0) : millis_(seconds * 1000.0) {};
    void update() {};
    float getInput(int index = 0) {
      return
        (sin(2.0 * 3.14 * (millis() % millis_) / (float)millis_) + 1.0)
        / 2.0;
    }
  private:
    int millis_;
};

class RandomInput : public Input {
  public:
    RandomInput(){};
    void update() {};
    float getInput(int index = 0) { return (float) rand() / (float) RAND_MAX; }
};

class SwitchInput : public Input {
  public:
    SwitchInput(int pin) : pin_(pin) { pinMode(pin_, INPUT); };
    void update() {};
    float getInput(int index = 0) { return digitalRead(pin_) == HIGH; }
  private:
    int pin_;
    bool released_;
};

// This will return 1.0 only once per press by keeping track of when the button
// is released
class ButtonInput : public Input {
  public:
    ButtonInput(int pin) : pin_(pin), released_(true) {};
    void update() {};
    float getInput(int index = 0) {
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

} // end namespace pixl
