void sceneWhiteBreath(int cue) {

  #ifdef SERIAL_DEBUG
    Serial.print("White breath");
    if (thisIsANewCue) Serial.println("new cue");
  #endif

  // disable the led strip
  if (thisIsANewCue){
     setLEDs(CRGB::Black);
  }
  
  // throb as breath is received
  int brightness = MIN_BREATH_BRIGHTNESS + ((255-MIN_BREATH_BRIGHTNESS) * triangleWave(getBreathPhase()));
  CHSV c = CHSV(0,0,brightness);
  heroLED(c);

}
