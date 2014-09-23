void sceneColour(int cue) {
  
#ifdef SERIAL_DEBUG
  Serial.println("Colour");
#endif

  // shared setup
  CRGB c = CRGB::Green; //placeholder
  setLEDs(c);
  heroLED(c);

  // cue specific
  switch (cue) {
    default:
      break;
  }

  // shared again

}
