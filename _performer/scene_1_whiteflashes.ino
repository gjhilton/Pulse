
void sceneWhiteFlashes(int cue) {

#ifdef SERIAL_DEBUG
  Serial.println("White flashes");
#endif

  // shared setup
  CHSV c = CHSV(0,0,255);
  heroLED(c);
  setLEDs(c);

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
  
  // comon again
  
}
