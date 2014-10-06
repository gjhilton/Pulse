unsigned long cueTime = 0;
int flashLength = 0;
int blackLength = 0;
float swfBrightness = 255;

void sceneWhiteFlashes(int cue) {

  #ifdef SERIAL_DEBUG
    Serial.print("White flashes");
    if (thisIsANewCue) Serial.println(" new cue");
  #endif

  // every cue
  if (thisIsANewCue){
    setLEDs(CRGB::Black); // led strip off
  }
  
  // cue specific
  switch (cue) {
    // cues 0,1,2 flash for a random length followed by a decreasing dark interval
    case 0:
        swfBrightness = 50;
        blackLength = 4000;
        flash();
        break;
    case 1:
        swfBrightness = 90;
        blackLength = 1000;
        flash();
      break;
    case 2:
        swfBrightness = 150;
        blackLength = 500;
        flash();
      break;
    // cue 3 eveybody white
    case 3:
        heroLED(CRGB::White);
        break;
    // cue 4 fade down for breath
    case 4:
      if (swfBrightness > MIN_BREATH_BRIGHTNESS) {
       swfBrightness --;
       CHSV c = CHSV(0,0,swfBrightness);
       heroLED(c);
      }
      break;
  }  
}

void flash(){
  if (thisIsANewCue){
    flashLength = (random(4) + 1) * 1000;
    cueTime = millis();
  }
  unsigned long now = millis();
  if (now < (cueTime + flashLength)) {
      CHSV c = CHSV(0,0,swfBrightness);
      heroLED(c);
  } else {
    if (now < (cueTime + flashLength  + blackLength)){
      heroLED(CRGB::Black);
    } else {
      cueTime = millis();
    }
  }
}
