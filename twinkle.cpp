#include "pixl.h"

using namespace pixl;

namespace pixl {

TwinkleVisualization::TwinkleVisualization(Input* input, int size)
    : Visualization(input, size) {

  // Set up positions array
  positions_ = new bool[size_];
  for (int i = 0; i < size_; i++) {
    positions_[i] = false;
  }

  // Setup up particles array
  randomSeed(micros());
  num_particles_ = size / 3;
  particles_ = new Particle[num_particles_];
  for (int i = 0; i < num_particles_; i++) {
    int brightness = random(0, 255);
    bool direction = random(0, 255) < 128 ? false : true;
    int position = randomFreePosition(0);

    particles_[i] = Particle {
      brightness,
      direction,
      position
    };
  }
}

TwinkleVisualization::~TwinkleVisualization() {
  delete[] positions_;
  delete[] particles_;
}

int TwinkleVisualization::randomFreePosition(int position) {
  positions_[position] = false;

  bool found = false;
  while (!found) {
    position = random(0, size_ - 1);
    if (positions_[position] == false) {
      found = true;
    }
  }

  positions_[position] = true;

  return position;
}

void TwinkleVisualization::update() {
  // Update particles
  for (int i = 0; i < num_particles_; i++) {
    Particle* p = &particles_[i];

    if (p->direction) {
      p->brightness += 4;
      if (p->brightness > 255) {
        p->brightness = 255;
        p->direction = false;
      }
    } else {
      p->brightness -= 4;

      if (p->brightness <= 0) {
        int new_position = randomFreePosition(p->position);

        p->brightness = 0;
        p->direction = true;
        randomSeed(micros());
        p->position = new_position;
      }
    }
  }

  // Blank the viz
  for (int i = 0; i < size_; i++) {
    viz_[i] = CRGB::Black;
  }

  // Apply particles to visualization
  for (int i = 0; i < num_particles_; i++) {
    Particle p = particles_[i];
    viz_[p.position] = CRGB::White;
    viz_[p.position].fadeToBlackBy(255 - p.brightness);
  }
}

} // end namespace pixl
