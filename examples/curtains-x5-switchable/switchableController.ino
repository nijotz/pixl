char buffer[16];
int len = 0;
int size = 25;
int smooth = 1;
bool rippleFreq = true;

void runSerial(Input* input, AudioControlSGTL5000 audioShield) {
  if (Serial.available() > 0) {
    // read the incoming byte:
    int incomingByte = Serial.read();
    buffer[len++] = incomingByte;
    if (len >= 16) {
      len = 0; // handle overflows
    }
    if (incomingByte == '\n') {

      if (buffer[0] == '1') {
        setupAnim(new RippleVisualization(input, size, smooth, rippleFreq));
      } else if (buffer[0] == '2') {
        setupAnim(new FireVisualization(input, size));
      } else if (buffer[0] == '3') {
        setupAnim(new TwinkleVisualization(input, size));
      } else if (buffer[0] == 'a') {
        if (buffer[1] == '0') {
          audioShield.inputSelect(AUDIO_INPUT_LINEIN);
        } else {
          audioShield.inputSelect(AUDIO_INPUT_MIC);
        }
      } else if (buffer[0] == 'b') {
        int brightness = 128;
        sscanf(buffer, "b%d", &brightness);
        FastLED.setBrightness(brightness);
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
}

