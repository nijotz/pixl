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

// Curtain LEDs
LEDs leds1 = LEDs(&strip1, 0, 150);
LEDs leds2 = LEDs(&strip2, 0, 150);
LEDs leds3 = LEDs(&strip3, 0, 150);
LEDs leds4 = LEDs(&strip4, 0, 150);

Visualization* viz;
CurtainAnimation* anim1;
CurtainAnimation* anim2;
CurtainAnimation* anim3;
CurtainAnimation* anim4;

// Audio shield setup
AudioInputI2S audio;
//AudioAnalyzePeak peak;
//AudioAnalyzeNoteFrequency note;
//AudioConnection patchCord1(audio, peak);
//AudioConnection patchCord2(audio, note);
AudioControlSGTL5000 audioShield;
AudioAnalyzeFFT1024 fft;
AudioConnection patchCord1(audio, 0, fft, 0);

void setup() {
  Log.Init(LOGLEVEL, 9600);
  delay(1000);
  Log.Info("Starting setup()\n");
  Serial.flush();
  delay(1000);

  AudioMemory(12);
  audioShield.enable();
  //audioShield.inputSelect(AUDIO_INPUT_LINEIN);
  //audioShield.lineInLevel(15);
  audioShield.inputSelect(AUDIO_INPUT_MIC);
  audioShield.micGain(45);
  //note.begin(.99);

  fft.windowFunction(AudioWindowHanning1024);

  //input = new AudioShieldInput(&peak, &note);
  input = new FFTInput(&fft);

  viz = new RippleVisualization(input, 35, 1, true);

  anim1 = new CurtainAnimation(viz, leds1);
  anim2 = new CurtainAnimation(viz, leds2);
  anim3 = new CurtainAnimation(viz, leds3);
  anim4 = new CurtainAnimation(viz, leds4);

  // Strip 1
  anim1->init(
     150,    // height in pixels
     1,     // width in pixels
     1.0,   // height in ratio of visualiation
     0.17,  // width in ratio of visualiation
     0.0,   // rotation in radians
    -0.5,   // x distance from visualization start in ratio of visualization
     0.0,   // y "
     0.0);  // z "

  // Strip 2
  anim2->init(
     150,    // height in pixels
     1,     // width in pixels
     1.0,   // height in ratio of visualiation
     0.17,  // width in ratio of visualiation
     0.0,   // rotation in radians
    -0.5,  // x distance from visualization start in ratio of visualization
     0.0,   // y "
     0.0);  // z "

  // Strip 3
  anim3->init(
     150,    // height in pixels
     1,     // width in pixels
     1.0,   // height in ratio of visualiation
     0.17,  // width in ratio of visualiation
     0.0,   // rotation in radians
    -0.5,  // x distance from visualization start in ratio of visualization
     0.0,   // y "
     0.0);  // z "

  // Strip 4
  anim4->init(
     25,    // height in pixels
     6,     // width in pixels
     1.0,   // height in ratio of visualiation
     0.17,  // width in ratio of visualiation
     0.0,   // rotation in radians
    -0.5,  // x distance from visualization start in ratio of visualization
     0.0,   // y "
     0.0);  // z "

  FastLED.addLeds<WS2811,  6, RGB>(strip1.leds, 150);
  FastLED.addLeds<WS2811,  5, RGB>(strip2.leds, 150);
  FastLED.addLeds<WS2811,  20, RGB>(strip3.leds, 150);
  FastLED.addLeds<WS2811,  21, RGB>(strip4.leds, 150);

  FastLED.setBrightness(255);

  Looper* looper = Looper::instance();
  looper->addInput(input);
  looper->addVisualization(viz);
  looper->addAnimation(anim1);
  looper->addAnimation(anim2);
  looper->addAnimation(anim3);
  looper->addAnimation(anim4);
  looper->setUpdatesPerSecond(40);

  Log.Info("Finished setup()\n");
  delay(100);
}

void loop() {
  AudioNoInterrupts();
  Looper::instance()->loop();
  AudioInterrupts();
}
