#include "pixl.h"
#include <FastLED.h>
#include <Logging.h>

#define LOGLEVEL LOG_LEVEL_INFOS
#define NUM_LEDS 27
#define COOLING 50
#define SPARKING 120

using namespace pixl;

//WaveInput* input = new WaveInput();
//HueVisualization viz = HueVisualization(input);
//MarchAnimation march = MarchAnimation(&viz, leds);

Input* input;
Visualization* viz;
Animation* anim;
LEDStrip strip = LEDStrip(NUM_LEDS);
LEDs leds = LEDs(&strip, 0, NUM_LEDS);

void setup() {
  Log.Init(LOGLEVEL, 9600);
  Log.Info("setup()");
  Serial.flush();

  input = new MSGEQ7Input(
    1,    // left pin
    0,    // right pin
    12,   // strobe pin
    13,   // reset pin,
    200,  // max amplitude
    20,   // min amplutide
    0,    // min freq
    1,    // max freq
    true, // use amplitude
    true  // mono
  );

  // input = new WaveInput();
  viz = new FireVisualization(input, NUM_LEDS);
  anim = new PassThroughAnimation(viz, leds);


  FastLED.addLeds<WS2811, 6, RGB>(strip.leds, NUM_LEDS);

  Looper* looper = Looper::instance();
  looper->setUpdatesPerSecond(60);
  looper->setFramesPerSecond(30);
  looper->addInput(input);
  looper->addVisualization(viz);
  looper->addAnimation(anim);

  Log.Debug("Finished setup()");
}

void loop() {
  Looper::instance()->loop();
}
