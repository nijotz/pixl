#include "triangle.h"
#include <Logging.h>

using namespace pixelmap;

TriangleAnimation::TriangleAnimation(Visualization* viz, LEDs** leds, int num_leds)
    : leds_(leds),
      num_leds_(num_leds),
      viz_(viz) {}

TriangleAnimation::~TriangleAnimation() {
  delete[] leds_;
  delete[] mapping_;
}

void TriangleAnimation::init() {}
void TriangleAnimation::init(double scale, bool outer) {
  Log.Info("Setting up Triangle animation\n");

  if (outer) {
    init_outer(scale);
  } else {
    init_inner(scale);
  }

  Log.Info("Finished setting up Triangle animation\n");
}

void TriangleAnimation::init_outer(double scale) {
  float viz_scaled = viz_->getSize() * scale;
  int length = leds_[0]->length();
  int side = length / 3;
  int half_side = length / 6;
  double sin_60 = 0.866;
  double sin_30 = 0.5;
  double cos_30 = 0.866;
  double height = sin_60 * side;

  Log.Debug("Initializing outer triangle\n");
  Log.Debug("Length: %d\n", length);
  Log.Debug("Side: %d\n", side);
  Log.Debug("Half Side: %d\n", half_side);

  mapping_ = new double[length];

  // For the top side of the outer triangle
  for (int i = 0; i <= half_side; i++) {
    // Doesn't matter left vs right, they get the same values
    int left_side = side + i;
    int right_side = (2 * side) - i;
    double a_squared = pow(height + 5, 2);
    double b_squared = pow(half_side - i, 2);
    double top_mapping =  sqrt(a_squared + b_squared) / viz_scaled;

    mapping_[left_side] = top_mapping;
    mapping_[right_side] = top_mapping;
  }

  // For the bottom sides of the outer triangle
  for (int i = 0; i <= side; i++) {
    int left_side = i;
    int right_side = length - 1 - i;

    double from_bottom = (double)i * cos_30;
    double from_viz_start = (2.0 * (double)height) + 5.0 - from_bottom;
    double from_midline = (double)i * sin_30;
    double a_squared = pow(from_viz_start, 2);
    double b_squared = pow(from_midline, 2);
    double bottom_mapping = sqrt(a_squared + b_squared) / viz_scaled;

    mapping_[left_side] = bottom_mapping;
    mapping_[right_side] = bottom_mapping;
  }
}

void TriangleAnimation::init_inner(double scale) {

  double viz_scaled = viz_->getSize() * scale;
  int length = leds_[0]->length();
  int side = length / 3;
  int half_side = length / 6;
  double sin_60 = 0.866;
  double height = sin_60 * side;
  double square_height = pow(height, 2);

  Log.Debug("Initializing inner triangle\n");
  Log.Debug("Length: %d\n", length);
  Log.Debug("Side: %d\n", side);
  Log.Debug("Half Side: %d\n", half_side);

  mapping_ = new double[length];

  // For side coming from the center
  for (int i = 0; i <= side; i++) {
    double side_mapping = (double)i / viz_scaled;
    mapping_[i] = side_mapping;
    mapping_[length - 1 - i] = side_mapping;
  }

  // For the bottom side of the inner triangle
  for (int i = 0; i <= half_side; i++) {
    // A^2 + B^2 = C^2
    // A = HEIGHT
    // B = HALF_SIDE
    // C = HYPOTONUSE -- where on the sound wave array to pull values from

    // Doesn't matter left vs right, they get the same values
    int left_side = side + i;
    int right_side = 2 * side - i;
    double bottom_mapping = sqrt(square_height + pow(half_side - i, 2)) / viz_scaled;

    mapping_[right_side] = bottom_mapping;
    mapping_[left_side] = bottom_mapping;
  }
}

void TriangleAnimation::update() {}

void TriangleAnimation::draw(float interpolation) {
  for (int i = 0; i < num_leds_; i++) {
    LEDs* leds = leds_[i];
    for (int j = 0; j < leds->length(); j++) {
      float viz_index = mapping_[j] * (float)viz_->getSize();
      (*leds)[j] = viz_->viz[(int)viz_index];
    }
  }
}

