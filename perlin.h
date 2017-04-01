#pragma once
#include "input.h"
#include "visualization.h"

namespace pixl {

class PerlinVisualization : public Visualization {
  public:
    PerlinVisualization(Input* input, int size = 1);
    void update();
};

} // end namespace pixl
