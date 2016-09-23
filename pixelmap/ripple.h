#pragma once
#include "input.h"
#include "visualization.h"

namespace pixelmap {

template <typename Type>
void PushStack(Type* array, int length, Type value);

class RippleVisualization : public Visualization {
  public:
    RippleVisualization(Input* input, int size = 1, int smoothing = 1);
    ~RippleVisualization();
    void update();
  private:
    void pushRipples(float value);
    double* smoothing_;
    int smoothing_length_;
};

} // end namespace pixelmap
