#include "pixl.h"
#include <FastLED.h>
#include <Logging.h>

#define LOGLEVEL LOG_LEVEL_INFOS

using namespace pixl;

WaveInput input = WaveInput();
HueVisualization viz = HueVisualization(&input);
LEDStrip strip = LEDStrip(128);
LEDs leds = LEDs(&strip, 0, 128);
MarchAnimation march = MarchAnimation(&viz, leds);

void setup() {
  Log.Init(LOGLEVEL, 9600);
  Log.Info("setup()");

  Looper* looper = Looper::instance();
  looper->addInput(&input);
  looper->addVisualization(&viz);
  looper->addAnimation(&march);

  FastLED.addLeds<NEOPIXEL, 6>(strip.leds, 128);
  FastLED.show();

  Log.Debug("Finished setup()");
}

void loop() {
  Looper::instance()->loop();
}
