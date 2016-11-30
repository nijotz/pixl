#include <pixelmap.h>
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

using namespace pixelmap;

Input* input;

// Curtains
LEDStrip strip1 = LEDStrip(150);
LEDStrip strip2 = LEDStrip(150);
LEDStrip strip3 = LEDStrip(150);
LEDStrip strip4 = LEDStrip(150);
LEDStrip strip5 = LEDStrip(150);
LEDStrip strip6 = LEDStrip(150);

// Triangles strips, group of 3 triangles, split in hardware to make 3 groups
LEDStrip strip7 = LEDStrip(165);

// Curtain LEDs
LEDs leds1 = LEDs(&strip1, 0, 150);
LEDs leds2 = LEDs(&strip2, 0, 150);
LEDs leds3 = LEDs(&strip3, 0, 150);
LEDs leds4 = LEDs(&strip4, 0, 150);
LEDs leds5 = LEDs(&strip5, 0, 150);
LEDs leds6 = LEDs(&strip6, 0, 150);

// Inner triangles
int inner_starts[] = {37, 0, 19};
int inner_lengths[] = {18, 19, 18};

LEDStrip* strips7[] = {&strip7, &strip7, &strip7};

// Outer triangles
int outer1_starts[] = {92, 55, 74};
int outer1_lengths[] = {18, 19, 18};

int outer2_starts[] = {129, 147, 110};
int outer2_lengths[] = {18, 18, 19};

LEDs leds7 = LEDs(3, strips7, inner_starts, inner_lengths);
LEDs leds8 = LEDs(3, strips7, outer1_starts, outer1_lengths);
LEDs leds9 = LEDs(3, strips7, outer2_starts, outer2_lengths);

LEDs* inner_leds[] = {&leds7};
LEDs* outer_leds[] = {&leds8, &leds9};

Visualization* viz;
CurtainAnimation* anim1;
CurtainAnimation* anim2;
CurtainAnimation* anim3;
CurtainAnimation* anim4;
CurtainAnimation* anim5;
CurtainAnimation* anim6;

TriangleAnimation* anim7;
TriangleAnimation* anim8;

// Audio shield setup
AudioInputI2S audio;
AudioAnalyzePeak peak;
AudioAnalyzeNoteFrequency note;
AudioConnection patchCord1(audio, peak);
AudioConnection patchCord2(audio, note);
AudioControlSGTL5000 audioShield;

void setup() {
  Log.Init(LOGLEVEL, 9600);
  delay(1000);
  Log.Info("Starting setup()\n");
  Serial.flush();
  delay(1000);

  AudioMemory(30);
  audioShield.enable();
  audioShield.inputSelect(AUDIO_INPUT_MIC);
  audioShield.micGain(100);
  note.begin(.90);

  input = new AudioShieldInput(&peak, &note);

  viz = new RippleVisualization(input, 75, 1, true);

  anim1 = new CurtainAnimation(viz, leds1);
  anim2 = new CurtainAnimation(viz, leds2);
  anim3 = new CurtainAnimation(viz, leds3);
  anim4 = new CurtainAnimation(viz, leds4);
  anim5 = new CurtainAnimation(viz, leds5);
  anim6 = new CurtainAnimation(viz, leds6);

  anim7 = new TriangleAnimation(viz, inner_leds, 1);
  anim8 = new TriangleAnimation(viz, outer_leds, 2);

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

  // Far left curtain
  anim5->init(
     25,    // height in pixels
     6,     // width in pixels
     1.0,   // height in ratio of visualiation
     0.25,  // width in ratio of visualiation
     0.0,   // rotation in radians
    -0.7,   // x distance from visualization start in ratio of visualization
    -0.7,   // y "
     0.0);  // z "

  // Far left curtain
  anim6->init(
     25,    // height in pixels
     6,     // width in pixels
     1.0,   // height in ratio of visualiation
     0.25,  // width in ratio of visualiation
     0.0,   // rotation in radians
    -0.7,   // x distance from visualization start in ratio of visualization
    -0.7,   // y "
     0.0);  // z "

  anim7->init(1.0);
  anim8->init(0.6, true);

  FastLED.addLeds<WS2811, 21, RGB>(strip4.leds, 150);
  FastLED.addLeds<WS2811, 20, RGB>(strip3.leds, 150);
  FastLED.addLeds<WS2811, 14, RGB>(strip2.leds, 150);
  FastLED.addLeds<WS2811,  7, RGB>(strip1.leds, 150);

  FastLED.addLeds<WS2811,  2, GRB>(strip7.leds, 165);

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
  looper->addAnimation(anim7);
  looper->addAnimation(anim8);
  looper->setUpdatesPerSecond(30);

  Log.Info("Finished setup()\n");
  delay(100);
}

void loop() {
  AudioNoInterrupts();
  Looper::instance()->loop();
  AudioInterrupts();
}
