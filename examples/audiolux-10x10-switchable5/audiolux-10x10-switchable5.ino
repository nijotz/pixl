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
CurtainAnimation* anim6;

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
const int ledPin = 13;
int ledState = LOW;
long previousMillis = 0;
long interval = 500;

void setup() {
  //Log.Init(LOGLEVEL, 9600);
  delay(1000);
  Log.Info("Starting setup()\n");
  pinMode(ledPin, OUTPUT);
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

  FastLED.addLeds<WS2811, 14, RGB>(strip1.leds, 150);
  FastLED.addLeds<WS2811,  2, RGB>(strip2.leds, 150);
  FastLED.addLeds<WS2811, 21, RGB>(strip3.leds, 150);
  FastLED.addLeds<WS2811, 20, RGB>(strip4.leds, 150);
  FastLED.addLeds<WS2811,  6, RGB>(strip5.leds, 150);

  FastLED.setBrightness(255);

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
     
  looper = Looper::instance();
  
  looper->setUpdatesPerSecond(30);
    
  setupAnim(new TwinkleVisualization(input, 150));

  Log.Info("Finished setup()\n");
  delay(100);
}

void blink() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }
}

char buffer[16];
int len = 0;
int size = 25;
int smooth = 1;
bool rippleFreq = true;

void loop() {
  if (Serial.available() > 0) {
    // read the incoming byte:
    int incomingByte = Serial.read();
    buffer[len++] = incomingByte;
    if (len >= 16) {
      len = 0; // handle overflows
    }
    if (incomingByte == '\n') {

      blink();
      if (buffer == "1\n") {
        setupAnim(new RippleVisualization(input, size, smooth, rippleFreq));
      } else if (buffer == "2\n") {
        setupAnim(new FireVisualization(input, size));
      } else if (buffer == "3\n") {
        setupAnim(new TwinkleVisualization(input, size));
      } else if (buffer[0] == 's') {
        int n = sscanf(buffer, "s%d", &size);
        if (n != 1) {
          size = 25;
        }
      } else if (buffer[0] == 'v') {
        int vol = 0;
        if (sscanf(buffer, "v%d", &vol) == 1) {
          audioShield.micGain(vol);
          audioShield.lineInLevel(vol);
        }
      } else if (buffer[0] == 'm') { // smooth
        if (sscanf(buffer, "m%d", &smooth) != 1) {
          smooth = 1;
        }
      } else if (buffer[0] == 'f') {
        if (sscanf(buffer, "f%b", &rippleFreq) != 1) {
          rippleFreq = true;
        }
      }
      len = 0;
    }
  }

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
  looper->addAnimation(anim5);

}
