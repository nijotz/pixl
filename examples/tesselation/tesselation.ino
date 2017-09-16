#include "pixl.h"
#include <FastLED.h>
#include <Logging.h>

#define LOGLEVEL LOG_LEVEL_INFOS

using namespace pixl;

WaveInput input = WaveInput(1);
RippleVisualization viz = RippleVisualization(&input, 20, 2);
LEDStrip strip = LEDStrip(64);
LEDs leds = LEDs(&strip, 0, 64);
TesselationAnimation tess = TesselationAnimation(&viz, leds);

void setup() {
  Log.Init(LOGLEVEL, 9600);
  Log.Info("setup()");

  tess.init(0.785, 0.5, 0.25, 0.25);

  Looper* looper = Looper::instance();
  looper->addInput(&input);
  looper->addVisualization(&viz);
  looper->addAnimation(&tess);

  FastLED.addLeds<P9813, 6, 4, RGB>(strip.leds, 64);
  FastLED.setBrightness(64);
  FastLED.show();

  Log.Debug("Finished setup()");
}

void loop() {
  Looper::instance()->loop();
}
