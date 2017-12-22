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
//LEDStrip strip1 = LEDStrip(720);
LEDStrip strip2 = LEDStrip(720);

// Curtain LEDs
//LEDs leds1 = LEDs(&strip1, 0, 720);
LEDs leds2 = LEDs(&strip2, 0, 720);

Visualization* viz;
SpokesAnimation* anim1;
SpokesAnimation* anim2;

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

  //FastLED.addLeds<WS2811, 14, RGB>(strip1.leds, 720);
  FastLED.addLeds<WS2811,  2, RGB>(strip2.leds, 720);
  //FastLED.addLeds<WS2811, 21, RGB>(strip3.leds, 720);
  //FastLED.addLeds<WS2811, 20, RGB>(strip4.leds, 720);
  //FastLED.addLeds<WS2811,  6, RGB>(strip5.leds, 720);

  FastLED.setBrightness(200);
  viz = new RippleVisualization(input, 35, 1, true);
  //anim1 = new SpokesAnimation(viz, leds1);
  anim2 = new SpokesAnimation(viz, leds2);
  //anim3 = new SpokesAnimation(viz, leds3);
  //anim4 = new SpokesAnimation(viz, leds4);
  //anim5 = new SpokesAnimation(viz, leds5);

  // Far right curtain
  //anim1->init(1.0);

  // Inner right curtain
  anim2->init(1.0);
  
  // Middle - spokes
  //anim3->init(1.0);  // scale

  // Inner left curtain
  //anim4->init(1.0);
  
  // Far left curtain
  //anim5->init(1.0);
  
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
  //anim1->setVisualization(viz);
  anim2->setVisualization(viz);
  //anim3->setVisualization(viz);
  //anim4->setVisualization(viz);
  //anim5->setVisualization(viz);
  
  looper->clearAll();
  looper->addInput(input);
  looper->addVisualization(viz);
  //looper->addAnimation(anim1);
  looper->addAnimation(anim2);
  //looper->addAnimation(anim3);
  //looper->addAnimation(anim4);
  //looper->addAnimation(anim5);

}
