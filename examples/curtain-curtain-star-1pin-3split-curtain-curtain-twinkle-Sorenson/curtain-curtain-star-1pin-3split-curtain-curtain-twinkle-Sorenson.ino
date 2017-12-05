#include <pixl.h>
#include <FastLED.h>
#include <Logging.h>

// Uncomment to turn off assertions
#define NDEBUG

// Weird thing I had to do to get the Logging library working with teensy
extern "C"{
  int _getpid(){ return -1;}
  int _kill(int pid, int sig){ return -1; }
  int _write(){return -1;}
}

#define LOGLEVEL LOG_LEVEL_INFOS

using namespace pixl;

Input* input;

// Curtains
LEDStrip strip1 = LEDStrip(150);
LEDStrip strip2 = LEDStrip(150);
LEDStrip strip3 = LEDStrip(150);
LEDStrip strip4 = LEDStrip(150);

// Triangles strips, group of 3 triangles, split in hardware to make 3 groups
LEDStrip strip5 = LEDStrip(165);

LEDStrip* strips[] = {&strip1, &strip2, &strip3, &strip4, &strip5};
int starts[] = {0, 0, 0, 0, 0, 0, 0};
int lengths[] = {150, 150, 150, 150, 150, 150, 165};
LEDs leds = LEDs(5, strips, starts, lengths);

Visualization* viz;
PassThroughAnimation* anim;

void setup() {
  Log.Init(LOGLEVEL, 9600);
  delay(1000);
  Log.Info("Starting setup()\n");
  Serial.flush();
  delay(1000);

  input = new RandomInput();
  viz = new TwinkleVisualization(input, 765);
  anim = new PassThroughAnimation(viz, leds);

  FastLED.addLeds<WS2811, 21, RGB>(strip4.leds, 150);
  FastLED.addLeds<WS2811, 20, RGB>(strip3.leds, 150);
  FastLED.addLeds<WS2811, 14, RGB>(strip2.leds, 150);
  FastLED.addLeds<WS2811,  7, RGB>(strip1.leds, 150);

  FastLED.addLeds<WS2811,  2, GRB>(strip5.leds, 165);

  FastLED.setBrightness(255);

  Looper* looper = Looper::instance();
  looper->addInput(input);
  looper->addVisualization(viz);
  looper->addAnimation(anim);
  looper->setUpdatesPerSecond(30);

  Log.Info("Finished setup()\n");
  delay(100);
}

void loop() {
  AudioNoInterrupts();
  Looper::instance()->loop();
  AudioInterrupts();
}
