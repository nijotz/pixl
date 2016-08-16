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

Input* input = new WaveInput(2.0);
LEDStrip strip = LEDStrip(150);
LEDs leds = LEDs(&strip, 0, 150);

Visualization* viz = new RippleVisualization(input, 151);
CurtainAnimation* anim = new CurtainAnimation(viz, leds);

void setup() {
  Log.Init(LOGLEVEL, 9600);
  Log.Info("Starting setup()");
  delay(100);

  anim->init(
     25,    // height in pixels
     6,     // width in pixels
     1.0,   // height in ratio of visualiation
     0.3,   // width in ratio of visualiation
     0.0,   // rotation in radians
    -0.175, // x distance from visualization start in ratio of visualization
    -0.5,   // y "
     0.0);  // z "

  FastLED.addLeds<WS2811, 6, RGB>(strip.leds, 151);
  FastLED.show();

  Looper* looper = Looper::instance();
  looper->addInput(input);
  looper->addVisualization(viz);
  looper->addAnimation(anim);

  Log.Info("Finished setup()");
  delay(100);
}

void loop() {
  Looper::instance()->loop();
}
