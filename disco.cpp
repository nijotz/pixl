#include <FastLED.h>
#include "disco.h"

using namespace pixl;

void DiscoAnimation::draw(float interp) {
  // First, we black out all the LEDs
  leds_.fillSolid(CRGB::Black);

  // To achive the strobe effect, we actually only draw lit pixels
  // every Nth frame (e.g. every 4th frame).
  // sStrobePhase is a counter that runs from zero to kStrobeCycleLength-1,
  // and then resets to zero.
  const uint8_t kStrobeCycleLength = 4; // light every Nth frame
  static uint8_t sStrobePhase = 0;
  sStrobePhase = sStrobePhase + 1;
  if (sStrobePhase >= kStrobeCycleLength ) {
    sStrobePhase = 0;
  }

  // We only draw lit pixels when we're in strobe phase zero;
  // in all the other phases we leave the LEDs all black.
  if (sStrobePhase == 0 ) {

    // The dash spacing cycles from 4 to 9 and back, 8x/min (about every 7.5 sec)
    uint8_t dashperiod= beatsin8( 8/*cycles per minute*/, 4,10);
    // The width of the dashes is a fraction of the dashperiod, with a minimum of one pixel
    uint8_t dashwidth = (dashperiod / 4) + 1;

    // The distance that the dashes move each cycles varies
    // between 1 pixel/cycle and half-the-dashperiod/cycle.
    // At the maximum speed, it's impossible to visually distinguish
    // whether the dashes are moving left or right, and the code takes
    // advantage of that moment to reverse the direction of the dashes.
    // So it looks like they're speeding up faster and faster to the
    // right, and then they start slowing down, but as they do it becomes
    // visible that they're no longer moving right; they've been
    // moving left.  Easier to see than t o explain.
    //
    // The dashes zoom back and forth at a speed that 'goes well' with
    // most dance music, a little faster than 120 Beats Per Minute.  You
    // can adjust this for faster or slower 'zooming' back and forth.
    uint8_t zoomBPM = 120;
    int8_t  dashmotionspeed = beatsin8( (zoomBPM /2), 1,dashperiod);
    // This is where we reverse the direction under cover of high speed
    // visual aliasing.
    if( dashmotionspeed >= (dashperiod/2)) {
      dashmotionspeed = 0 - (dashperiod - dashmotionspeed );
    }


    // The hueShift controls how much the hue of each dash varies from
    // the adjacent dash.  If hueShift is zero, all the dashes are the
    // same color. If hueShift is 128, alterating dashes will be two
    // different colors.  And if hueShift is range of 10..40, the
    // dashes will make rainbows.
    // Initially, I just had hueShift cycle from 0..130 using beatsin8.
    // It looked great with very low values, and with high values, but
    // a bit 'busy' in the middle, which I didnt like.
    //   uint8_t hueShift = beatsin8(2,0,130);
    //
    // So instead I layered in a bunch of 'cubic easings'
    // (see http://easings.net/#easeInOutCubic )
    // so that the resultant wave cycle spends a great deal of time
    // "at the bottom" (solid color dashes), and at the top ("two
    // color stripes"), and makes quick transitions between them.
    uint8_t cycle = beat8(2); // two cycles per minute
    uint8_t easedcycle = ease8InOutCubic( ease8InOutCubic( cycle));
    uint8_t wavecycle = cubicwave8( easedcycle);
    uint8_t hueShift = scale8( wavecycle,130);


    // Each frame of the animation can be repeated multiple times.
    // This slows down the apparent motion, and gives a more static
    // strobe effect.  After experimentation, I set the default to 1.
    uint8_t strobesPerPosition = 1; // try 1..4


    // Now that all the parameters for this frame are calculated,
    // we call the 'worker' function that does the next part of the work.
    discoWorker( dashperiod, dashwidth, dashmotionspeed, strobesPerPosition, hueShift);
  }
}


// discoWorker updates the positions of the dashes, and calls the draw function
//
void DiscoAnimation::discoWorker(
    uint8_t dashperiod, uint8_t dashwidth, int8_t  dashmotionspeed,
    uint8_t stroberepeats,
    uint8_t huedelta)
 {
  static uint8_t sRepeatCounter = 0;
  static int8_t sStartPosition = 0;
  static uint8_t sStartHue = 0;

  // Always keep the hue shifting a little
  sStartHue += 1;

  // Increment the strobe repeat counter, and
  // move the dash starting position when needed.
  sRepeatCounter = sRepeatCounter + 1;
  if( sRepeatCounter>= stroberepeats) {
    sRepeatCounter = 0;

    sStartPosition = sStartPosition + dashmotionspeed;

    // These adjustments take care of making sure that the
    // starting hue is adjusted to keep the apparent color of
    // each dash the same, even when the state position wraps around.
    if( sStartPosition >= dashperiod ) {
      while( sStartPosition >= dashperiod) { sStartPosition -= dashperiod; }
      sStartHue  -= huedelta;
    } else if( sStartPosition < 0) {
      while( sStartPosition < 0) { sStartPosition += dashperiod; }
      sStartHue  += huedelta;
    }
  }

  // draw dashes with full brightness (value), and somewhat
  // desaturated (whitened) so that the LEDs actually throw more light.
  const uint8_t kSaturation = 208;
  const uint8_t kValue = 255;

  // call the function that actually just draws the dashes now
  drawRainbowDashes( sStartPosition, leds_.length()-1,
                     dashperiod, dashwidth,
                     sStartHue, huedelta,
                     kSaturation, kValue);
}


// drawRainbowDashes - draw rainbow-colored 'dashes' of light along the led strip:
//   starting from 'startpos', up to and including 'lastpos'
//   with a given 'period' and 'width'
//   starting from a given hue, which changes for each successive dash by a 'huedelta'
//   at a given saturation and value.
//
//   period = 5, width = 2 would be  _ _ _ X X _ _ _ Y Y _ _ _ Z Z _ _ _ A A _ _ _
//                                   \-------/       \-/
//                                   period 5      width 2
//
void DiscoAnimation::drawRainbowDashes(
  uint8_t startpos, uint16_t lastpos, uint8_t period, uint8_t width,
  uint8_t huestart, uint8_t huedelta, uint8_t saturation, uint8_t value)
{
  uint8_t hue = huestart;
  for (uint16_t i = startpos; i <= lastpos; i += period) {
    CRGB color = CHSV(hue, saturation, value);

    // draw one dash
    uint16_t pos = i;
    for (uint8_t w = 0; w < width; w++) {
      leds_[pos] = color;
      pos++;
      if (pos >= leds_.length()) {
        break;
      }
    }

    hue += huedelta;
  }
}

DiscoAnimation::DiscoAnimation(Visualization* viz, LEDs& leds)
  : leds_(leds),
    viz_(viz) {}

void DiscoAnimation::init() {}
void DiscoAnimation::update() {}
