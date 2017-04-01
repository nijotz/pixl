#include "pixl.h"
#include <FastLED.h>
#include <Logging.h>

#define LOGLEVEL LOG_LEVEL_INFOS

using namespace pixl;

WaveInput input = WaveInput();
PerlinVisualization viz = PerlinVisualization(&input, 75);
LEDStrip strip = LEDStrip(75);
LEDs leds = LEDs(&strip, 0, 75);
PassThroughAnimation anim = PassThroughAnimation(&viz, leds);

void setup() {
  Log.Init(LOGLEVEL, 9600);
  Log.Info("setup()");

  Looper* looper = Looper::instance();
  looper->addInput(&input);
  looper->addVisualization(&viz);
  looper->addAnimation(&anim);

  FastLED.addLeds<NEOPIXEL, 6>(strip.leds, 75);
  FastLED.show();

  Log.Debug("Finished setup()");
}

void loop() {
  Looper::instance()->loop();
}
