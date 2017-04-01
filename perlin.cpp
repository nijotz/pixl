#include "pixl.h"
#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

using namespace pixl;

namespace pixl {

PerlinVisualization::PerlinVisualization(Input* input, int size)
    : Visualization(input, size) {
}

void PerlinVisualization::update() {
  for (int i = 0; i < size_; i ++) {
    float coord = (float)i * 3.0 / (float)size_;
    float time = (millis() % 8192) / 8192.0;
    float val = stb_perlin_noise3(coord, coord, time, 0, 0, 8192);
    val = abs(val);
    CRGB color = CHSV(val * 255, 255, 255);
    viz_[i] = color;
  }
}

} // end namespace pixl
