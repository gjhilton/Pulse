float sceneTestBrightness = 0;

void sceneTest(int cue) {

  #ifdef SERIAL_DEBUG
    Serial.println("Test scene");
  #endif

  // do for all cues
  CHSV c;

  // cue specific
  switch (cue) {
    case 0:
      // cue ONE => brightness controlled by potentiometer
      sceneTestBrightness = lastNormalisedParameter()*255;
      c = CHSV(0,0,sceneTestBrightness);
      break;
    default:
      // cue TWO => colour controlled by potentiometer, brightness whatever it was set to at end of previous scene
      c = CHSV(lastNormalisedParameter()*255,255,sceneTestBrightness);
      break;
  }

  // do for all cues
  setLEDs(c);
  heroLED(c);
}
