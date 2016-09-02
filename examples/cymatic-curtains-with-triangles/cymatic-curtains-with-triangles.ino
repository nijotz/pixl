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

// Curtains
LEDStrip strip1 = LEDStrip(150);
LEDStrip strip2 = LEDStrip(150);
LEDStrip strip3 = LEDStrip(150);
LEDStrip strip4 = LEDStrip(150);

// Triangles strips, groups of 3 triangles
LEDStrip strip5 = LEDStrip(165);
LEDStrip strip6 = LEDStrip(165);
LEDStrip strip7 = LEDStrip(165);

// Curtain LEDs
LEDs leds1 = LEDs(&strip1, 0, 150);
LEDs leds2 = LEDs(&strip2, 0, 150);
LEDs leds3 = LEDs(&strip3, 0, 150);
LEDs leds4 = LEDs(&strip4, 0, 150);

// Inner triangles
int inner_starts[] = {37, 0, 19};
int inner_lengths[] = {18, 19, 18};

LEDStrip* strips5[] = {&strip5, &strip5, &strip5};
LEDStrip* strips6[] = {&strip6, &strip6, &strip6};
LEDStrip* strips7[] = {&strip7, &strip7, &strip7};

LEDs leds5 = LEDs(3, strips5, inner_starts, inner_lengths);
LEDs leds6 = LEDs(3, strips6, inner_starts, inner_lengths);
LEDs leds7 = LEDs(3, strips7, inner_starts, inner_lengths);

LEDs* inner_leds[] = {&leds5, &leds6, &leds7};

// Outer triangles
int outer1_starts[] = {92, 55, 74};
int outer1_lengths[] = {18, 19, 18};

int outer2_starts[] = {147, 110, 129};
int outer2_lengths[] = {18, 19, 18};

LEDs leds8 = LEDs(3, strips5, outer1_starts, outer1_lengths);
LEDs leds9 = LEDs(3, strips6, outer1_starts, outer1_lengths);
LEDs leds10 = LEDs(3, strips7, outer1_starts, outer1_lengths);

LEDs leds11 = LEDs(3, strips5, outer2_starts, outer2_lengths);
LEDs leds12 = LEDs(3, strips6, outer2_starts, outer2_lengths);
LEDs leds13 = LEDs(3, strips7, outer2_starts, outer2_lengths);

LEDs* outer_leds[] = {&leds8, &leds9, &leds10, &leds11, &leds12, &leds13};

Visualization* viz;
CurtainAnimation* anim1;
CurtainAnimation* anim2;
CurtainAnimation* anim3;
CurtainAnimation* anim4;

TriangleAnimation* anim5;
TriangleAnimation* anim6;

void setup() {
  Log.Init(LOGLEVEL, 9600);
  delay(1000);
  Log.Info("Starting setup()\n");
  Serial.flush();
  delay(1000);

  //input = new WaveInput();
  input = new MSGEQ7Input(
    1,    // left pin
    0,    // right pin
    12,   // strobe pin
    13,   // reset pin,
    1000, // max amplitude
    250   // min amplutide
  );

  viz = new RippleVisualization(input, 75, 1);

  anim1 = new CurtainAnimation(viz, leds1);
  anim2 = new CurtainAnimation(viz, leds2);
  anim3 = new CurtainAnimation(viz, leds3);
  anim4 = new CurtainAnimation(viz, leds4);

  anim5 = new TriangleAnimation(viz, inner_leds, 3);
  anim6 = new TriangleAnimation(viz, outer_leds, 6);

  // Far right curtain
  anim1->init(
     25,    // height in pixels
     6,     // width in pixels
     1.0,   // height in ratio of visualiation
     0.25,  // width in ratio of visualiation
     0.0,   // rotation in radians
     0.45,  // x distance from visualization start in ratio of visualization
    -0.7,   // y "
     0.0);  // z "

  // Middle right curtain
  anim2->init(
     25,    // height in pixels
     6,     // width in pixels
     1.0,   // height in ratio of visualiation
     0.25,  // width in ratio of visualiation
     0.0,   // rotation in radians
     0.13,   // x distance from visualization start in ratio of visualization
    -0.5,   // y "
     0.0);  // z "

  // Middle left curtain
  anim3->init(
     25,    // height in pixels
     6,     // width in pixels
     1.0,   // height in ratio of visualiation
     0.25,  // width in ratio of visualiation
     0.0,   // rotation in radians
    -0.38,  // x distance from visualization start in ratio of visualization
    -0.5,   // y "
     0.0);  // z "

  // Far left curtain
  anim4->init(
     25,    // height in pixels
     6,     // width in pixels
     1.0,   // height in ratio of visualiation
     0.25,  // width in ratio of visualiation
     0.0,   // rotation in radians
    -0.7,   // x distance from visualization start in ratio of visualization
    -0.7,   // y "
     0.0);  // z "

  anim5->init(1.0);
  anim6->init(0.6, true);

  FastLED.addLeds<WS2811,  8, RGB>(strip1.leds, 150);
  FastLED.addLeds<WS2811,  9, RGB>(strip2.leds, 150);
  FastLED.addLeds<WS2811, 10, RGB>(strip3.leds, 150);
  FastLED.addLeds<WS2811, 11, RGB>(strip4.leds, 150);

  FastLED.addLeds<WS2811,  4, GRB>(strip5.leds, 165);
  FastLED.addLeds<WS2811,  6, GRB>(strip6.leds, 165);
  FastLED.addLeds<WS2811,  7, GRB>(strip7.leds, 165);

  FastLED.setBrightness(255);

  Looper* looper = Looper::instance();
  looper->addInput(input);
  looper->addVisualization(viz);
  looper->addAnimation(anim1);
  looper->addAnimation(anim2);
  looper->addAnimation(anim3);
  looper->addAnimation(anim4);
  looper->addAnimation(anim5);
  looper->addAnimation(anim6);

  Log.Info("Finished setup()\n");
  delay(100);
}

void loop() {
  Looper::instance()->loop();
}
