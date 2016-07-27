#include "pixelmap.h"

using namespace pixelmap;

namespace pixelmap {

HueVisualization::HueVisualization(Input* input)
  : Visualization(input),
    input(input) { }

void HueVisualization::update() {
  float val = input->getInput();
  viz[0] = CRGB(val * 255, 255, 255);
}

} // end namespace pixelmap
