#include "pixelmap/pixelmap.h"
#include <FastLED.h>
#include <Logging.h>

#define LOGLEVEL LOG_LEVEL_INFOS

using namespace pixelmap;

RandomInput input = RandomInput();
LEDStrip strip = LEDStrip(128);
LEDs leds = LEDs(strip, 0, 128);

Visualization* viz;
Animation* anim;

ButtonInput button = ButtonInput(5);

int currentAnimation = 0;
bool firstRun = true;

void setup() {
  Log.Init(LOGLEVEL, 9600);
  Log.Info("setup()");

  FastLED.addLeds<NEOPIXEL, 6>(strip.leds, 128);
  FastLED.show();

  Log.Debug("Finished setup()");
}

void loop() {
  Looper* looper = Looper::instance();

  if (button.getInput() > 0.5 || firstRun == true) {
    Log.Info("Button press detected");
    switch(currentAnimation) {
      case 0:
        Log.Info("Switching to Disco animation");
        looper->clearAll();
        delete viz;
        delete anim;
        viz = new NullVisualization(&input, 128);
        anim = new DiscoAnimation(viz, leds);
        looper->addInput(&input);
        looper->addVisualization(viz);
        looper->addAnimation(anim);
        currentAnimation = 1;
        break;
      case 1:
        Log.Info("Switching to Fire animation");
        looper->clearAll();
        delete viz;
        delete anim;
        viz = new FireVisualization(&input, 128);
        anim = new PassThroughAnimation(viz, leds);
        looper->addInput(&input);
        looper->addVisualization(viz);
        looper->addAnimation(anim);
        currentAnimation = 0;
        break;
    }
  }

  firstRun = false;

  looper->loop();
}
