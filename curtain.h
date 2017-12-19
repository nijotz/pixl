#pragma once
#include "animation.h"
#include "leds.h"
#include "visualization.h"

namespace pixl {

class CurtainAnimation : public Animation {
  public:
    CurtainAnimation(Visualization* viz, LEDs& leds);
    ~CurtainAnimation();
    void init();
    void init(int height_px, int width_px,
              double height_r, double width_r,
              double rotation_rad, double x_r, double y_r, double z_r);
    void update();
    void draw(float interpolation);
    void setVisualization(Visualization* viz);

  private:
    LEDs& leds_;
    Visualization* viz_;
    double* mapping_;
};

} // end namespace pixl
