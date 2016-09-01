#pragma once
#include "input.h"
#include "visualization.h"

namespace pixelmap {

class OnOffVisualization : public Visualization {
  public:
    OnOffVisualization(Input* input, int smoothing);
    ~OnOffVisualization();
    void update();
  private:
    double* smoothing_;
    int smoothing_length_;
};

} // end namespace pixelmap
