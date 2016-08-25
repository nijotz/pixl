#pragma once
#include <Logging.h>
#include "animation.h"
#include "input.h"
#include "visualization.h"

namespace pixelmap {

/* The Looper is a singleton that is responsible for calling update() and
   draw() on Inputs, Visualizations, and Animations */
class Looper {
  public:
    static Looper* instance() {
      if (!looper_) {
        looper_ = new Looper;
      }
      return looper_;
    }
    void loop();
    void addAnimation(Animation* animation);
    void addInput(Input* input);
    void addVisualization(Visualization* visualization);
    void clearAll();
    void setUpdatesPerSecond(int updates);

  private:
    Looper()
      : num_anim_(0),
        num_input_(0),
        num_viz_(0),
        next_update_tick_(0),
        next_draw_tick_(0),
        draw_millis_per_tick_(0),
        update_millis_per_tick_(0)
    {
      Log.Info("Looper instantiated\n");
    }
    void update_();
    void draw_(float interpolation);
    static Looper* looper_;
    int updates_per_second_;
    unsigned long next_update_tick_;
    unsigned long next_draw_tick_;
    float draw_millis_per_tick_;
    float update_millis_per_tick_;
    int num_anim_;
    int num_input_;
    int num_viz_;
    Animation* animations_[10];
    Visualization* visualizations_[10];
    Input* inputs_[10];
};

} // end namespace pixelmap
