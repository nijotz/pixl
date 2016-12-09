#include <pixelmap.h>
#include <FastLED.h>
#include <Logging.h>

// Uncomment to turn off assertions
//#define NDEBUG

// Weird thing I had to do to get the Logging library working with teensy
extern "C"{
  int _getpid(){ return -1;}
  int _kill(int pid, int sig){ return -1; }
  int _write(){return -1;}
}

#define LOGLEVEL LOG_LEVEL_DEBUG

using namespace pixelmap;

Input* input;

// Curtains
LEDStrip strip1 = LEDStrip(150);
LEDStrip strip2 = LEDStrip(150);
LEDStrip strip3 = LEDStrip(150);
LEDStrip strip4 = LEDStrip(150);
LEDStrip strip5 = LEDStrip(150);

// Curtain LEDs
LEDs leds1 = LEDs(&strip1, 0, 150);
LEDs leds2 = LEDs(&strip2, 0, 150);
LEDs leds3 = LEDs(&strip3, 0, 150);
LEDs leds4 = LEDs(&strip4, 0, 150);
LEDs leds5 = LEDs(&strip5, 0, 150);

Visualization* viz;
CurtainAnimation* anim1;
CurtainAnimation* anim2;
CurtainAnimation* anim3;
CurtainAnimation* anim4;
CurtainAnimation* anim5;

// Twinkle LEDs
LEDStrip* strips[] = {&strip1, &strip2, &strip3, &strip4, &strip5};
int starts[] = {0, 0, 0, 0, 0, 0, 0};
int lengths[] = {150, 150, 150, 150, 150, 150, 165};
LEDs twinkle_leds = LEDs(5, strips, starts, lengths);

// Twinkle animation
Visualization* twinkle_viz;
PassThroughAnimation* twinkle_anim;

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
  audioShield.inputSelect(AUDIO_INPUT_LINEIN);
  audioShield.lineInLevel(15);
  //audioShield.inputSelect(AUDIO_INPUT_MIC);
  //audioShield.micGain(100);
  //note.begin(.99);

  fft.windowFunction(AudioWindowHanning1024);

  input = new FFTInput(&fft);

  viz = new RippleVisualization(input, 35, 1, true);

  twinkle_viz = new TwinkleVisualization(input, 750);
  twinkle_anim = new PassThroughAnimation(twinkle_viz, twinkle_leds);

  anim1 = new CurtainAnimation(viz, leds1);
  anim2 = new CurtainAnimation(viz, leds2);
  anim3 = new CurtainAnimation(viz, leds3);
  anim4 = new CurtainAnimation(viz, leds4);
  anim5 = new CurtainAnimation(viz, leds5);

  // Far right curtain
  anim1->init(
     25,    // height in pixels
     6,     // width in pixels
     1.0,   // height in ratio of visualiation
     0.33,  // width in ratio of visualiation
     0.0,   // rotation in radians
     0.50,  // x distance from visualization start in ratio of visualization
    -0.5,   // y "
     0.0);  // z "

  // Inner right curtain
  anim2->init(
     25,    // height in pixels
     6,     // width in pixels
     1.0,   // height in ratio of visualiation
     0.33,  // width in ratio of visualiation
     0.0,   // rotation in radians
     0.17,  // x distance from visualization start in ratio of visualization
    -0.5,   // y "
     0.0);  // z "

  // Middle curtain
  anim3->init(
     25,    // height in pixels
     6,     // width in pixels
     1.0,   // height in ratio of visualiation
     0.33,  // width in ratio of visualiation
     0.0,   // rotation in radians
    -0.17, // x distance from visualization start in ratio of visualization
    -0.5,   // y "
     0.0);  // z "

  // Inner left curtain
  anim4->init(
     25,    // height in pixels
     6,     // width in pixels
     1.0,   // height in ratio of visualiation
     0.33,  // width in ratio of visualiation
     0.0,   // rotation in radians
    -0.50,  // x distance from visualization start in ratio of visualization
    -0.5,   // y "
     0.0);  // z "

  // Far left curtain
  anim5->init(
     25,    // height in pixels
     6,     // width in pixels
     1.0,   // height in ratio of visualiation
     0.33,  // width in ratio of visualiation
     0.0,   // rotation in radians
    -0.88,  // x distance from visualization start in ratio of visualization
    -0.5,   // y "
     0.0);  // z "

  FastLED.addLeds<WS2811, 14, RGB>(strip1.leds, 150);
  FastLED.addLeds<WS2811,  2, RGB>(strip2.leds, 150);
  FastLED.addLeds<WS2811, 21, RGB>(strip3.leds, 150);
  FastLED.addLeds<WS2811, 20, RGB>(strip4.leds, 150);
  FastLED.addLeds<WS2811,  6, RGB>(strip5.leds, 150);

  FastLED.setBrightness(255);

  Looper* looper = Looper::instance();
  looper->addInput(input);
  looper->addVisualization(viz);
  looper->addAnimation(anim1);
  looper->addAnimation(anim2);
  looper->addAnimation(anim3);
  looper->addAnimation(anim4);
  looper->addAnimation(anim5);
  looper->setUpdatesPerSecond(30);

  Log.Info("Finished setup()\n");
  delay(1000);
}

bool switched = false;
void loop() {
  Looper* looper = Looper::instance();

  AudioNoInterrupts();
  looper->loop();
  AudioInterrupts();

  if (millis() > 10000 && switched == false) {
    switched = true;
    looper->clearAll();
    looper->addVisualization(twinkle_viz);
    looper->addAnimation(twinkle_anim);
  }
}
