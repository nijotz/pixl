#pragma once
#include "animation.h"
#include "leds.h"
#include "visualization.h"

namespace pixl {

class FireVisualization : public Visualization {
  public:
    FireVisualization(Input* input, int size = 1);
    ~FireVisualization();
    void update();
  private:
    byte* heat_;
};

} // end namespace pixl
