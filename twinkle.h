#pragma once
#include "input.h"
#include "visualization.h"

namespace pixl {

struct Particle {
  int brightness;
  bool direction;
  int position;
};

class TwinkleVisualization : public Visualization {
  public:
    TwinkleVisualization(Input* input, int size = 1);
    ~TwinkleVisualization();
    void update();
  private:
    int randomFreePosition(int position);
    bool* positions_;
    Particle* particles_;
    int num_particles_;
};

} // end namespace pixl
