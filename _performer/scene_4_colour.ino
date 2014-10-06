unsigned long startMillis = 0;

void sceneColour(int cue) {

#ifdef SERIAL_DEBUG
  Serial.print("Colour");
  if (thisIsANewCue) Serial.println("new cue");
#endif

  // cue specific
  switch (cue) {
    case 0:
      black();
      break;
    case 1:
      both(CHSV(171, 255, 255)); // turquoise
      break;
    case 2:
      black();
      break;
    case 3:
      both(CHSV(32, 255, 255)); // yellow
      break;
    case 4:
      if (thisIsANewCue) {
        startMillis = millis();
      }
      sparkleFade((millis() - startMillis) / 1000.0, 1, 50, 10);
      break;
    case 5:
      if (thisIsANewCue) {
        startMillis = millis();
      }
      sparkleFade((millis() - startMillis) / 1000.0, 51, 50, 10);
      break;
    case 6:
      if (thisIsANewCue) {
        startMillis = millis();
      }
      sparkleFade((millis() - startMillis) / 1000.0, 101, 50, 10);
      break;
    case 7:
      if (thisIsANewCue) {
        startMillis = millis();
      }
      sparkleFade((millis() - startMillis) / 1000.0, 151, 50, 10);
      break;
    case 8:
      colouredBreath();
      break;
  }

}

void colouredBreath() {
  float breath = getBreathPhase();
  int brightness = MIN_BREATH_BRIGHTNESS + ((255 - MIN_BREATH_BRIGHTNESS) * triangleWave(breath));
  float hue = 100 + (100 * triangleWave(1-breath));
  CHSV c = CHSV(hue, 255, brightness);
  setLEDs(c);
  heroLED(c);
}

void black() {
  const CRGB c = CRGB::Black;
  setLEDs(c);
  heroLED(c);
}

void both(CRGB c) {
  setLEDs(c);
  heroLED(c);
}

void sparkleFade(float elapsedSeconds, int startHue, int hueChange, float durationSeconds) {
  if (elapsedSeconds < durationSeconds) {
    int hue = int(interpolate(elapsedSeconds, startHue, hueChange, durationSeconds, LINEAR));
    CHSV c = CHSV(hue, 255, 255);
    setLEDs(c);
    heroLED(c);
  }
}
