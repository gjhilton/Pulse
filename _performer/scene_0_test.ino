void sceneTest(int cue) {

  
#ifdef SERIAL_DEBUG
  Serial.println("Test scene");
#endif

const int TESTMODE_BRIGHTNESS = 255; // use low brightness to save power

  // shared setup
  CHSV c = CHSV(lastNormalisedParameter()*255,255,TESTMODE_BRIGHTNESS);
  setLEDs(c);
  heroLED(c);
  // cue specific
  switch (cue) {
    default:
      break;
  }

  // shared again

}
