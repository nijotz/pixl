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

#define LOGLEVEL LOG_LEVEL_ERRORS

using namespace pixl;

Input* input;

// Curtains
LEDStrip strip1 = LEDStrip(200);
LEDStrip strip2 = LEDStrip(400);
LEDStrip strip3 = LEDStrip(400);
LEDStrip strip4 = LEDStrip(400);

// Curtain LEDs
LEDs leds1 = LEDs(&strip1, 0, 200);
LEDs leds2 = LEDs(&strip2, 0, 400);
LEDs leds3 = LEDs(&strip3, 0, 400);
LEDs leds4 = LEDs(&strip4, 0, 400);


Visualization* viz;
CurtainAnimation* anim1;
SpokesAnimation* anim2;
SpokesAnimation* anim3;
SpokesAnimation* anim4;

Looper* looper;

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
  Log.Init(LOGLEVEL, 115200);
  delay(1000);
  Log.Info("Starting setup()\n");
  Serial.flush();
  delay(1000);

  AudioMemory(12);
  audioShield.enable();
  //audioShield.inputSelect(AUDIO_INPUT_LINEIN);
  //audioShield.lineInLevel(15);
  audioShield.inputSelect(AUDIO_INPUT_MIC);
  audioShield.micGain(63);
  //audioShield.lineInLevel(15);
  //note.begin(.99);

  fft.windowFunction(AudioWindowHanning1024);

  input = new FFTInput(&fft);

  FastLED.addLeds<WS2811,  6, RGB>(strip1.leds, 200); // Spokes 1 
  FastLED.addLeds<WS2811,  14, GRB>(strip2.leds, 400); // Spokes 2
  FastLED.addLeds<WS2811,  21, GRB>(strip3.leds, 400); // Spokes 3
  FastLED.addLeds<WS2811,  2, GRB>(strip4.leds, 400); // Spokes 4


  FastLED.setBrightness(200);
  viz = new RippleVisualization(input, 35, 1, true);
  anim1 = new CurtainAnimation(viz, leds1); // Spokes
  anim2 = new SpokesAnimation(viz, leds2); // Spokes
  anim3 = new SpokesAnimation(viz, leds3); // Spokes
  anim4 = new SpokesAnimation(viz, leds4); // Spokes

  // Inner right  curtain
  anim1->init(
     25,    // height in pixels
     6,     // width in pixels
     1.0,   // height in ratio of visualiation
     0.17,  // width in ratio of visualiation
     0.0,   // rotation in radians
     0.25,   // x distance from visualization start in ratio of visualization
    -0.5,   // y "
     0.0);  // z "

  // Spokes 2
  anim2->init(0.5);  // scale

  // Spokes 3
  anim3->init(0.5);  // scale

  // Spokes 4
  anim4->init(0.5);  // scale
  
     
  looper = Looper::instance();
  
  looper->setUpdatesPerSecond(30);
    
  setupAnim(viz);

  Log.Info("Finished setup()\n");
  delay(100);
}

void loop() {
  runSerial(input, audioShield);
  
  AudioNoInterrupts();
  Looper::instance()->loop();
  AudioInterrupts();
}

void setupAnim(Visualization* viz) {
  anim1->setVisualization(viz);
  anim2->setVisualization(viz);
  anim3->setVisualization(viz);
  anim4->setVisualization(viz);
  
  looper->clearAll();
  looper->addInput(input);
  looper->addVisualization(viz);
  looper->addAnimation(anim1);
  looper->addAnimation(anim2);
  looper->addAnimation(anim3);
  looper->addAnimation(anim4);

}
