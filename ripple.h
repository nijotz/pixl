#pragma once
#include "input.h"
#include "visualization.h"

namespace pixl {

template <typename Type>
void PushStack(Type* array, int length, Type value);

class RippleVisualization : public Visualization {
  public:
    RippleVisualization(Input* input, int size = 1, int smoothing = 1, bool freq = false);
    ~RippleVisualization();
    void update();
  private:
    void update_freq();
    void update_amp();
    void pushRipples(float value);
    float* smoothing_amp_;
    float* smoothing_freq_;
    int smoothing_length_;
    bool freq_;
};

} // end namespace pixl
