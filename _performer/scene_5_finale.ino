void sceneFinale(int cue) {
  
#ifdef SERIAL_DEBUG
  Serial.println("finale");
#endif

  // shared setup
  CRGB c = CRGB::Purple; //placeholder
  setLEDs(c);
  heroLED(c);

  // cue specific
  switch (cue) {
    default:
      break;
  }

  // shared again

}
