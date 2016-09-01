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

Input* input;

LEDStrip strip = LEDStrip(150);
LEDs leds = LEDs(&strip, 0, 150);

Visualization* viz;
CurtainAnimation* anim;

void setup() {
  Log.Init(LOGLEVEL, 9600);
  delay(1000);
  Log.Info("Starting setup()\n");
  Serial.flush();
  delay(1000);

  //input = new NoteInput();
  input = new MSGEQ7Input(
    1,    // left pin
    0,    // right pin
    12,   // strobe pin
    13,   // reset pin,
    1000, // max amplitude
    250   // min amplutide
  );

  viz = new RippleVisualization(input, 1, 0);

  anim = new CurtainAnimation(viz, leds);

  anim->init(
     25,    // height in pixels
     6,     // width in pixels
     1.0,   // height in ratio of visualiation
     0.25,  // width in ratio of visualiation
     0.0,   // rotation in radians
     0.25,  // x distance from visualization start in ratio of visualization
    -0.5,   // y "
     0.0);  // z "

  FastLED.addLeds<WS2811, 6, RGB>(strip.leds, 150);
  FastLED.show();

  Looper* looper = Looper::instance();
  looper->addInput(input);
  looper->addVisualization(viz);
  looper->addAnimation(anim);

  Log.Info("Finished setup()\n");
  delay(100);
}

void loop() {
  Looper::instance()->loop();
}
