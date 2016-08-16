#pragma once
#include "input.h"
#include "visualization.h"

namespace pixelmap {

template <typename Type>
void PushStack(Type* array, int length, Type value);

class RippleVisualization : public Visualization {
  public:
    RippleVisualization(Input* input, int size = 1);
    void update();
  private:
    void pushRipples(float value);
    float smoothing_[2];
    int smoothing_length_ = 2;
};

} // end namespace pixelmap
