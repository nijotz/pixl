#pragma once

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

} // end namespace pixelmap
