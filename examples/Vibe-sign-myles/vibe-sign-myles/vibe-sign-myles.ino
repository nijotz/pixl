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

#define LOGLEVEL LOG_LEVEL_DEBUG

using namespace pixl;

Input* input;

int triangle_length = 135;
int circle_length = 143;
LEDStrip strip = LEDStrip(278);

// Triangle
LEDs triangle_led = LEDs(&strip, 143, triangle_length);
LEDs* triangle_leds[] = {&triangle_led};
TriangleAnimation* triangle_anim;

// Circle
LEDs circle_led = LEDs(&strip, 0, circle_length);
LEDs* circle_leds[] = {&circle_led};
CircleAnimation* circle_anim;

Visualization* viz;

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

  AudioMemory(10);
  audioShield.enable();
  audioShield.inputSelect(AUDIO_INPUT_LINEIN);
  audioShield.lineInLevel(15);
  //audioShield.inputSelect(AUDIO_INPUT_MIC);
  //audioShield.micGain(25);
  //note.begin(.99);

  fft.windowFunction(AudioWindowHanning1024);

  //input = new AudioShieldInput(&peak, &note);
  input = new FFTInput(&fft);
  //input = new WaveInput();

  viz = new RippleVisualization(input, 50, 1, true);
  triangle_anim = new TriangleAnimation(viz, triangle_leds, 1);
  circle_anim = new CircleAnimation(viz, circle_leds, 1);

  triangle_anim->init(1.0);
  circle_anim->init(1.0);

  FastLED.addLeds<WS2811, 2, GRB>(strip.leds, 278);
  FastLED.setBrightness(255);

  Looper* looper = Looper::instance();
  looper->addInput(input);
  looper->addVisualization(viz);
  looper->addAnimation(triangle_anim);
  looper->addAnimation(circle_anim);
  looper->setUpdatesPerSecond(30);

  Log.Info("Finished setup()\n");
  delay(100);
}

void loop() {
  AudioNoInterrupts();
  Looper::instance()->loop();
  AudioInterrupts();
}
