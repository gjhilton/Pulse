void sceneWhiteBreath(int cue) {

#ifdef SERIAL_DEBUG
  Serial.println("White breath");
#endif

  // shared setup
  int breath = getBreathPhase();
  int brightness = 255 * triangleWave(getBreathPhase());
  CHSV c = CHSV(0,0,brightness);
  setLEDs(c);
  heroLED(c);

  // cue specific
  switch (cue) {
    case 1:

      break;
    case 2:
    
      break;
    case 3:

      break;
    case 4:

      break;
  }

}
