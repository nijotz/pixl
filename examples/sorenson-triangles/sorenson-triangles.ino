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

LEDStrip strip = LEDStrip(165);

// Inner triangles
int inner_starts[] = {37, 0, 19};
int inner_lengths[] = {18, 19, 18};

LEDStrip* strips[] = {&strip, &strip, &strip};

// Outer triangles
int outer1_starts[] = {92, 55, 74};
int outer1_lengths[] = {18, 19, 18};

int outer2_starts[] = {129, 147, 110};
int outer2_lengths[] = {18, 18, 19};

LEDs leds1 = LEDs(3, strips, inner_starts, inner_lengths);
LEDs leds2 = LEDs(3, strips, outer1_starts, outer1_lengths);
LEDs leds3 = LEDs(3, strips, outer2_starts, outer2_lengths);

LEDs* inner_leds[] = {&leds1};
LEDs* outer_leds[] = {&leds2, &leds3};

Visualization* viz;
TriangleAnimation* anim1;
TriangleAnimation* anim2;

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

  //input = new AudioShieldInput(&peak, &note);
  input = new FFTInput(&fft);

  viz = new RippleVisualization(input, 165, 1, true);
  anim1 = new TriangleAnimation(viz, inner_leds, 1);
  anim2 = new TriangleAnimation(viz, outer_leds, 2);

  anim1->init(1.0);
  anim2->init(1.0, true);

  FastLED.addLeds<WS2811, 2, GRB>(strip.leds, 165);

  FastLED.setBrightness(255);

  Looper* looper = Looper::instance();
  looper->addInput(input);
  looper->addVisualization(viz);
  looper->addAnimation(anim1);
  looper->addAnimation(anim2);
  looper->setUpdatesPerSecond(30);

  Log.Info("Finished setup()\n");
  delay(100);
}

void loop() {
  AudioNoInterrupts();
  Looper::instance()->loop();
  AudioInterrupts();
}
