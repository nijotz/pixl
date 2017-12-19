#include "curtain.h"
#include <Assert.h>
#include <Logging.h>
#include <Math.h>

using namespace pixl;

CurtainAnimation::CurtainAnimation(Visualization* viz, LEDs& leds)
  : leds_(leds),
    viz_(viz)
{ }

CurtainAnimation::~CurtainAnimation() {
  delete[] mapping_;
}

void CurtainAnimation::init() {}
void CurtainAnimation::init(int height_px, int width_px,
                            double height_r, double width_r,
                            double rotation_rad,
                            double x_r, double y_r, double z_r)
{
  Log.Info("Setting up Curtain animation\n");
  int llen = leds_.length();
  Log.Info("LED count %d\n", (int)llen);
  assert( (height_px * width_px) <= llen );

  Log.Info("%d %d\n", height_px, width_px);
  mapping_ = new double[llen];
  Log.Info("Mapped");
  for (int i = 0; i < llen; i++) {
    Log.Info("LED # %d\n", (int)i);
    mapping_[i] = 0.0;
  }

  // Assumes LEDs start at the top right corner and then go down, left, back
  // up, left back down, etc.
  int x = width_px - 1;
  int y = 0;
  int y_inc = 1;

  for (int i = 0; i < llen; i++) {
    Log.Info("LED # %d\n", (int)i);
    // Find distance from top left corner (0,0)
    double x_distance = width_r * ((double)(x + 1) / (double)width_px);
    double y_distance = height_r * ((double)(y + 1) / (double)height_px);

    // Find distance from start of visualization
    x_distance += x_r;
    y_distance += y_r;

    double distance = sqrt( pow(x_distance, 2) + pow(y_distance, 2) );

    // Verify it's between 0 and 1
    distance = max(0.0, distance);
    distance = min(1.0, distance);

    // Add the mapping
    mapping_[i] = distance;

    // Calculate new x,y coordinates
    y += y_inc;
    if (y == height_px) {
      x--;
      y = height_px - 1;
      y_inc = -1;
    } else if (y < 0) {
      x--;
      y = 0;
      y_inc = 1;
    }
  }
}

void CurtainAnimation::setVisualization(Visualization* viz) {
  viz_ = viz;
}

void CurtainAnimation::update() {}

void CurtainAnimation::draw(float interpolation) {
  for (int i = 0; i < leds_.length(); i++) {
    leds_[i] = viz_->getColorByRatio(mapping_[i]);
  }
}
