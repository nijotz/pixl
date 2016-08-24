#define __ASSERT_USE_STDERR

#include "pixelmap/pixelmap.h"
#include <Assert.h>
#include <FastLED.h>
#include <Logging.h>

#define LOGLEVEL LOG_LEVEL_INFOS

#define BRIGHTNESS 150

#define STAFF_LEDS 100
#define RING_LEDS 12

#define BUTTON_PIN 5
#define STAFF_PIN 6
#define RING_PIN 7

using namespace pixelmap;

RandomInput input = RandomInput();
LEDStrip staff = LEDStrip(STAFF_LEDS);
LEDStrip ring = LEDStrip(RING_LEDS);

LEDStrip* strips[] = {&staff, &ring};
int starts[] = {0, 0};
int lengths[] = {STAFF_LEDS, RING_LEDS};

LEDs leds = LEDs(2, strips, starts, lengths, true);

Visualization* viz;
Animation* anim;

ButtonInput button = ButtonInput(BUTTON_PIN);

int currentAnimation = 0;
bool firstRun = true;

void setup() {
  Log.Init(LOGLEVEL, 9600);
  Log.Info("setup()");

  FastLED.addLeds<NEOPIXEL, STAFF_PIN>(staff.leds, STAFF_LEDS);
  FastLED.addLeds<NEOPIXEL, RING_PIN>(ring.leds, RING_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.show();

  Log.Debug("Finished setup()");
}

void loop() {
  Looper* looper = Looper::instance();

  if (button.getInput() > 0.5 || firstRun == true) {
    Log.Info("Button press detected");
    switch(currentAnimation) {
      case 0:
        Log.Info("Switching to Twinkle animation");
        looper->clearAll();
        delete viz;
        delete anim;
        viz = new TwinkleVisualization(&input, STAFF_LEDS);
        anim = new PassThroughAnimation(viz, leds);
        looper->addInput(&input);
        looper->addVisualization(viz);
        looper->addAnimation(anim);
        looper->setUpdatesPerSecond(50);
        currentAnimation = 1;
        break;
      case 1:
        Log.Info("Switching to Disco animation");
        looper->clearAll();
        delete viz;
        delete anim;
        viz = new NullVisualization(&input, STAFF_LEDS);
        anim = new DiscoAnimation(viz, leds);
        looper->addInput(&input);
        looper->addVisualization(viz);
        looper->addAnimation(anim);
        currentAnimation = 2;
        break;
      case 2:
        Log.Info("Switching to Fire animation");
        looper->clearAll();
        delete viz;
        delete anim;
        viz = new FireVisualization(&input, STAFF_LEDS);
        anim = new PassThroughAnimation(viz, leds);
        looper->addInput(&input);
        looper->addVisualization(viz);
        looper->addAnimation(anim);
        looper->setUpdatesPerSecond(35);
        currentAnimation = 0;
        break;
    }
  }

  firstRun = false;

  looper->loop();
}

// handle diagnostic informations given by assertion and abort program execution:
void __assert(const char *__func, const char *__file, int __lineno, const char *__sexp) {
    // transmit diagnostic informations through serial link.
    Serial.println(__func);
    Serial.println(__file);
    Serial.println(__lineno, DEC);
    Serial.println(__sexp);
    Serial.flush();
    delay(1000);

    // abort program execution.
    abort();
}
