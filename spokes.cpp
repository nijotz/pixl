#include "spokes.h"
#include <Assert.h>
#include <Logging.h>
#include <Math.h>

using namespace pixl;

SpokesAnimation::SpokesAnimation(Visualization* viz, LEDs& leds)
  : leds_(leds),
    viz_(viz)
{ }

SpokesAnimation::~SpokesAnimation() {
  delete[] mapping_;
}

void SpokesAnimation::init() {}
void SpokesAnimation::init(float scale)
{
  Log.Info("Setting up Spokes animation\n");
  
  int spokes = 12;
  int angle = 360 / spokes;
  int perStrip = 60;
  int llen = leds_.length();
  Log.Info("LED count %i\n", llen);

  mapping_ = new double[llen];
  for (int i = 0; i < llen; i++) {
    //Log.Info("LED # %d\n", (int)i);
    mapping_[i] = 0.0;
  }

  // Assume 60 LEDs per spoke, all start from the hub. Starting at 12 o'clock
  // each spoke is offset 30 degrees from the last.
  // 12 - 0-59      1 - 60 - 119        2 - 120 - 179
  //  3 - 180-239   4 - 240 - 299       5 - 300 - 359
  //  6 - 360-410   7 - 420 - 479       8 - 480 - 539
  //  9 - 540-599   10 - 600 - 659      11 - 660 - 719
  for (int iX = 0; iX < spokes; iX++) {
    for (int iY = 0; iY < perStrip; iY++) {
      //Log.Info("LED # %d %d\n", (int)iX, iY);
      mapping_[iX*perStrip+iY] = (float)iY/(float)perStrip * scale;
      //Log.Info("%i calc %s\n", iX*perStrip+iY, (float)iY/(float)perStrip * scale);
    }
  }
}

void SpokesAnimation::setVisualization(Visualization* viz) {
  viz_ = viz;
}

void SpokesAnimation::update() {}

void SpokesAnimation::draw(float interpolation) {
  for (int i = 0; i < leds_.length(); i++) {
    leds_[i] = viz_->getColorByRatio(mapping_[i]);
  }
}
