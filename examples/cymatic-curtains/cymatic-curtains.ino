#include "pixelmap/pixelmap.h"
#include <FastLED.h>
#include <Logging.h>

// Weird thing I had to do to get the Logging library working with teensy
extern "C"{
  int _getpid(){ return -1;}
  int _kill(int pid, int sig){ return -1; }
  int _write(){return -1;}
}

#define LOGLEVEL LOG_LEVEL_INFOS

using namespace pixelmap;

Input* input = new MSGEQ7Input(
  1,    // left pin
  0,    // right pin
  12,   // strobe pin
  13,   // reset pin,
  1000, // max amplitude
  250   // min amplutide
);
LEDStrip strip = LEDStrip(600);
LEDs leds1 = LEDs(&strip, 0, 150);
LEDs leds2 = LEDs(&strip, 150, 150);
LEDs leds3 = LEDs(&strip, 300, 150);
LEDs leds4 = LEDs(&strip, 450, 150);

Visualization* viz = new RippleVisualization(input, 150);
CurtainAnimation* anim1 = new CurtainAnimation(viz, leds1);
CurtainAnimation* anim2 = new CurtainAnimation(viz, leds2);
CurtainAnimation* anim3 = new CurtainAnimation(viz, leds3);
CurtainAnimation* anim4 = new CurtainAnimation(viz, leds4);

void setup() {
  Log.Init(LOGLEVEL, 9600);
  Log.Info("Starting setup()");
  delay(100);

  anim1->init(
     25,    // height in pixels
     6,     // width in pixels
     1.0,   // height in ratio of visualiation
     0.25,  // width in ratio of visualiation
     0.0,   // rotation in radians
     0.25,  // x distance from visualization start in ratio of visualization
    -0.5,   // y "
     0.0);  // z "

  anim2->init(
     25,    // height in pixels
     6,     // width in pixels
     1.0,   // height in ratio of visualiation
     0.25,  // width in ratio of visualiation
     0.0,   // rotation in radians
     0.0,   // x distance from visualization start in ratio of visualization
    -0.5,   // y "
     0.0);  // z "

  anim3->init(
     25,    // height in pixels
     6,     // width in pixels
     1.0,   // height in ratio of visualiation
     0.25,  // width in ratio of visualiation
     0.0,   // rotation in radians
    -0.25,  // x distance from visualization start in ratio of visualization
    -0.5,   // y "
     0.0);  // z "

  anim4->init(
     25,    // height in pixels
     6,     // width in pixels
     1.0,   // height in ratio of visualiation
     0.25,  // width in ratio of visualiation
     0.0,   // rotation in radians
    -0.5,   // x distance from visualization start in ratio of visualization
    -0.5,   // y "
     0.0);  // z "

  FastLED.addLeds<WS2811, 6, RGB>(strip.leds, 600);
  FastLED.show();

  Looper* looper = Looper::instance();
  looper->addInput(input);
  looper->addVisualization(viz);
  looper->addAnimation(anim1);
  looper->addAnimation(anim2);
  looper->addAnimation(anim3);
  looper->addAnimation(anim4);

  Log.Info("Finished setup()");
  delay(100);
}

void loop() {
  Looper::instance()->loop();
}
