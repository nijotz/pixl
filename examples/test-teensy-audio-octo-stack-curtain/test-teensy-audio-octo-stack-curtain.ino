#include <pixelmap.h>
#include <FastLED.h>
#include <Logging.h>
#include <Audio.h>

// Uncomment to turn off assertions
//#define NDEBUG

// Weird thing I had to do to get the Logging library working with teensy
extern "C"{
  int _getpid(){ return -1;}
  int _kill(int pid, int sig){ return -1; }
  int _write(){return -1;}
}

#define LOGLEVEL LOG_LEVEL_INFOS

using namespace pixelmap;

Input* input;

LEDStrip strip = LEDStrip(150);
LEDStrip strip2 = LEDStrip(150);
LEDStrip strip3 = LEDStrip(288);
LEDs leds1 = LEDs(&strip, 0, 150);
LEDs leds2 = LEDs(&strip2, 0, 150);
LEDs leds3 = LEDs(&strip3, 0, 288);

Visualization* viz;
CurtainAnimation* anim1;
CurtainAnimation* anim2;
CurtainAnimation* anim3;

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
  //audioShield.inputSelect(AUDIO_INPUT_MIC);
  audioShield.inputSelect(AUDIO_INPUT_LINEIN);
  audioShield.micGain(50);
  note.begin(.75);

  input = new AudioShieldInput(&peak, &note);

  viz = new RippleVisualization(input, 60, 1, true);

  anim1 = new CurtainAnimation(viz, leds1);
  anim2 = new CurtainAnimation(viz, leds2);
  anim3 = new CurtainAnimation(viz, leds3);
  delay(100);

  anim1->init(
     25,    // height in pixels
     6,     // width in pixels
     1.0,   // height in ratio of visualiation
     0.25,  // width in ratio of visualiation
     0.0,   // rotation in radians
    -0.125,  // x distance from visualization start in ratio of visualization
    -0.5,   // y "
     0.0);  // z "

  anim2->init(
     25,    // height in pixels
     6,     // width in pixels
     1.0,   // height in ratio of visualiation
     0.25,  // width in ratio of visualiation
     0.0,   // rotation in radians
    -0.125,  // x distance from visualization start in ratio of visualization
    -0.5,   // y "
     0.0);  // z "

  anim3->init(
     25,    // height in pixels
     6,     // width in pixels
     1.0,   // height in ratio of visualiation
     0.25,  // width in ratio of visualiation
     0.0,   // rotation in radians
    -0.125,  // x distance from visualization start in ratio of visualization
    -0.5,   // y "
     0.0);  // z "

  FastLED.addLeds<WS2811, 2, RGB>(strip.leds, 150);
  FastLED.addLeds<WS2811, 14, RGB>(strip2.leds, 150);
  FastLED.addLeds<WS2811, 7, RGB>(strip3.leds, 150);
  //FastLED.show();

  Looper* looper = Looper::instance();
  looper->addInput(input);
  looper->addVisualization(viz);
  looper->addAnimation(anim1);
  looper->addAnimation(anim2);
  looper->addAnimation(anim3);
  looper->setUpdatesPerSecond(30);

  Log.Info("Finished setup()\n");
  delay(1000);
}

void loop() {
  AudioNoInterrupts();
  Looper::instance()->loop();
  AudioInterrupts();
  delay(10);
}
