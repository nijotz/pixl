#include <pixl.h>
#include <FastLED.h>
#include <Logging.h>
#include <Audio.h>

// Uncomment to turn off assertions
#define NDEBUG

// Weird thing I had to do to get the Logging library working with teensy
extern "C" {
  int _getpid() { return -1; }
  int _kill(int pid, int sig) { return -1; }
  int _write() { return -1; }
}

#define LOGLEVEL LOG_LEVEL_INFOS

using namespace pixl;


// Triangles strip, in series, 3 groups of 3 triangles, inner -> outer -> outer
#define TRIANGLE 55
#define GROUP TRIANGLE * 3
#define STRIP GROUP * 3

//#define IN_START1 18
//#define IN_START2 36
//#define IN_START3 0
#define IN_START1 36
#define IN_START2 0
#define IN_START3 18
#define IN_LENGTH1 19
#define IN_LENGTH2 18
#define IN_LENGTH3 18

#define OUT1_START1 37
#define OUT1_START2 0
#define OUT1_START3 19
#define OUT1_LENGTH1 18
#define OUT1_LENGTH2 19
#define OUT1_LENGTH3 18

#define OUT2_START1 19
#define OUT2_START2 37
#define OUT2_START3 0
#define OUT2_LENGTH1 18
#define OUT2_LENGTH2 18
#define OUT2_LENGTH3 19

Input* input;

int circle_length = 30;

LEDStrip strip = LEDStrip(STRIP);
LEDStrip strip2 = LEDStrip(150);
LEDStrip* strips[] = {&strip, &strip, &strip};

// Inner triangles
int in_starts1[] = {
  IN_START1 + (GROUP * 0),
  IN_START2 + (GROUP * 0),
  IN_START3 + (GROUP * 0)
};
int in_starts2[] = {
  IN_START1 + (GROUP * 1),
  IN_START2 + (GROUP * 1),
  IN_START3 + (GROUP * 1)
};
int in_starts3[] = {
  IN_START1 + (GROUP * 2),
  IN_START2 + (GROUP * 2),
  IN_START3 + (GROUP * 2)
};
int in_lengths[] = {IN_LENGTH1, IN_LENGTH2, IN_LENGTH3};

LEDs leds1 = LEDs(3, strips, in_starts1, in_lengths);
LEDs leds2 = LEDs(3, strips, in_starts2, in_lengths);
LEDs leds3 = LEDs(3, strips, in_starts3, in_lengths);

LEDs* inner_leds[] = {&leds1, &leds2, &leds3};

// First outer triangle
int out1_starts1[] = {
  OUT1_START1 + TRIANGLE + (GROUP * 0),
  OUT1_START2 + TRIANGLE + (GROUP * 0),
  OUT1_START3 + TRIANGLE + (GROUP * 0)
};
int out1_starts2[] = {
  OUT1_START1 + TRIANGLE + (GROUP * 1),
  OUT1_START2 + TRIANGLE + (GROUP * 1),
  OUT1_START3 + TRIANGLE + (GROUP * 1)
};
int out1_starts3[] = {
  OUT1_START1 + TRIANGLE + (GROUP * 2),
  OUT1_START2 + TRIANGLE + (GROUP * 2),
  OUT1_START3 + TRIANGLE + (GROUP * 2)
};
int out1_lengths[] = {OUT1_LENGTH1, OUT1_LENGTH2, OUT1_LENGTH3};

LEDs leds4 = LEDs(3, strips, out1_starts1, out1_lengths);
LEDs leds5 = LEDs(3, strips, out1_starts2, out1_lengths);
LEDs leds6 = LEDs(3, strips, out1_starts3, out1_lengths);

// Second outer triangle
int out2_starts1[] = {
  OUT2_START1 + (TRIANGLE * 2) + (GROUP * 0),
  OUT2_START2 + (TRIANGLE * 2) + (GROUP * 0),
  OUT2_START3 + (TRIANGLE * 2) + (GROUP * 0)
};
int out2_starts2[] = {
  OUT2_START1 + (TRIANGLE * 2) + (GROUP * 1),
  OUT2_START2 + (TRIANGLE * 2) + (GROUP * 1),
  OUT2_START3 + (TRIANGLE * 2) + (GROUP * 1)
};
int out2_starts3[] = {
  OUT2_START1 + (TRIANGLE * 2) + (GROUP * 2),
  OUT2_START2 + (TRIANGLE * 2) + (GROUP * 2),
  OUT2_START3 + (TRIANGLE * 2) + (GROUP * 2)
};
int out2_lengths[] = {OUT2_LENGTH1, OUT2_LENGTH2, OUT2_LENGTH3};

LEDs leds7 = LEDs(3, strips, out2_starts1, out2_lengths);
LEDs leds8 = LEDs(3, strips, out2_starts2, out2_lengths);
LEDs leds9 = LEDs(3, strips, out2_starts3, out2_lengths);

LEDs* outer_leds[] = {&leds4, &leds5, &leds6, &leds7, &leds8, &leds9};

// Circle
LEDs circle_led = LEDs(&strip, 0, circle_length);
LEDs* circle_leds[] = {&circle_led};
CircleAnimation* circle_anim;

Visualization* viz;

TriangleAnimation* anim1;
TriangleAnimation* anim2;

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
  audioShield.micGain(50);
  note.begin(.75);

  input = new AudioShieldInput(&peak, &note);

  viz = new RippleVisualization(input, 150, 1, true);

  anim1 = new TriangleAnimation(viz, inner_leds, 3);
  anim2 = new TriangleAnimation(viz, outer_leds, 6);

  // Adds LED Microphone
  circle_anim = new CircleAnimation(viz, circle_leds, 1);

  anim1->init(1.0);
  anim2->init(0.9, true);
  circle_anim->init(1.0);

  FastLED.addLeds<WS2811, 14, GRB>(strip.leds, STRIP);
  FastLED.addLeds<WS2811,  7, RGB>(strip2.leds, 150);

  FastLED.setBrightness(255);

  Looper* looper = Looper::instance();
  looper->addInput(input);
  looper->addVisualization(viz);
  looper->addAnimation(anim1);
  looper->addAnimation(anim2);
  looper->addAnimation(circle_anim);
  //looper->setFramesPerSecond(30);
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
