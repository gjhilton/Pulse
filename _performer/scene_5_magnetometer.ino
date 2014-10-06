void sceneMagnetometer(int cue) {

#ifdef SERIAL_DEBUG
  Serial.print("Magnetometer");
  if (thisIsANewCue) Serial.println("new cue");
#endif

#ifdef USE_MAGNETOMETER
  CHSV c = CHSV(compass.read()*255, 255, 255);
  heroLED(c);
  setLEDs(c);
#endif

}
