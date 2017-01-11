#include "fire.h"

// Fire2012 by Mark Kriegsman, July 2012
// as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
////
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation,
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on size_; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking.
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100
#define COOLING  100

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 175

namespace pixl {

FireVisualization::FireVisualization(Input* input, int size)
  : Visualization(input, size)
  {
    // Array of temperature readings at each simulation cell
    heat_ = new byte[size];
  }

FireVisualization::~FireVisualization() {
  delete[] heat_;
}

void FireVisualization::update() {

  // Step 1.  Cool down every cell a little
  for (int i = 0; i < size_; i++) {
    heat_[i] = qsub8( heat_[i],  random8(0, ((COOLING * 10) / size_) + 2));
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for (int i = size_ - 1; i >= 2; i--) {
    heat_[i] = (heat_[i - 1] + heat_[i - 2] + heat_[i - 2] ) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if ( (int)(255.0 * input_->getInput()) < SPARKING ) {
    int i = random8(7);
    heat_[i] = qadd8( heat_[i], random8(160,255) );
  }

  // Step 4.  Map from heat cells to LED colors
  for (int i = 0; i < size_; i++) {
    CRGB color = HeatColor(heat_[i]);
    viz_[i] = color;
  }
}

} // end namespace pixl
