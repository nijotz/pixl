#include <pixelmap.h>
#include <FastLED.h>
#include <Logging.h>

// Weird thing I had to do to get the Logging library working with teensy
extern "C"{
  int _getpid(){ return -1;}
  int _kill(int pid, int sig){ return -1; }
  int _write(){return -1;}
}

#define LOGLEVEL LOG_LEVEL_DEBUG

using namespace pixelmap;

Input* input;

LEDStrip strip = LEDStrip(57);
LEDs leds = LEDs(&strip, 0, 57);
Visualization* viz;
TriangleAnimation* anim;

void setup() {
  Log.Init(LOGLEVEL, 9600);
  delay(1000);
  Log.Debug("Starting setup()\n");
  Serial.flush();
  delay(1000);

  input = new MSGEQ7Input(
    1,    // left pin
    0,    // right pin
    12,   // strobe pin
    13,   // reset pin,
    1000, // max amplitude
    250   // min amplutide
  );

  FastLED.addLeds<WS2811, 6, RGB>(strip.leds, 57);

  viz = new RippleVisualization(input, 150);
  anim = new TriangleAnimation(viz, leds);
  anim->init(0.4);

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
