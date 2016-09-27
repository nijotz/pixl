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

LEDStrip strip = LEDStrip(50);
LEDs leds = LEDs(&strip, 0, 50);
Visualization* viz;
ScaledAnimation* anim;

void setup() {
  Log.Init(LOGLEVEL, 9600);
  delay(1000);
  Log.Debug("Starting setup()\n");
  Serial.flush();
  delay(1000);

  input = new RandomInput();

  FastLED.addLeds<WS2811, 6, RGB>(strip.leds, 50);

  viz = new RippleVisualization(input, 50, 1);
  anim = new ScaledAnimation(viz, leds);

  Looper* looper = Looper::instance();
  looper->addInput(input);
  looper->addVisualization(viz);
  looper->addAnimation(anim);
  looper->setUpdatesPerSecond(10);

  FastLED.setBrightness(64);

  Log.Info("Finished setup()\n");
  delay(100);
}

void loop() {
  Looper::instance()->loop();
}
