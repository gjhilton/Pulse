void sceneMagnetometer(int cue) {
  
#ifdef SERIAL_DEBUG
  Serial.println("Magnetometer");
#endif

  // shared setup
  CHSV c = CHSV(compass.read()*255,255,255);
  heroLED(c);
  setLEDs(c);

  // cue specific
  switch (cue) {
    default:
      break;
  }

  // shared again

}
