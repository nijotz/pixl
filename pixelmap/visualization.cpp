#include "pixelmap.h"

using namespace pixelmap;

namespace pixelmap {

HueVisualization::HueVisualization(Input* input, int size)
  : Visualization(input) {}

void HueVisualization::update() {
  float val = input->getInput();
  viz[0] = CHSV(val * 255, 255, 255);
}

} // end namespace pixelmap
