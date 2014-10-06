bool upwards = true;
int lednum = 0;
int chasedelay = 0;

void sceneChase(int cue) {

#ifdef SERIAL_DEBUG
  Serial.println("chase");
  if (thisIsANewCue) Serial.println("new cue");
#endif
  if (thisIsANewCue) {
    heroLED(CRGB::Black);
  }

  // cue specific
  switch (cue) {
      // one pixel backwards and forwards
    case 0:
      onePixelBackwardsAndForwards();
      break;
    case 1:
      twoPixelsReflected(20);
      break;
    case 2:
      if (thisIsANewCue) {
        chasedelay = random(5)+1;
      }
      twoPixelsReflected(chasedelay);
      break;
    case 3:
      wings();
      break;
    case 4:
      if (thisIsANewCue) {
        setLEDs(CRGB::Black);
        leds[0] = CRGB::Red;
      }
      rotateLEDs(1, 0);
      FastLED.show();
      break;
    case 5:
      if (thisIsANewCue) {
        setLEDs(CRGB::Black);
        leds[0] = CRGB::Red;
        leds[NUM_LEDS / 2] = CRGB::Purple;

      }
      rotateLEDs(1, 0);
      FastLED.show();
      break;
    case 6:
      if (thisIsANewCue) {
        setLEDs(CRGB::Black);
        leds[0] = CRGB::Navy;
        leds[NUM_LEDS / 2] = CRGB::Navy;
        leds[NUM_LEDS / 4] = CRGB::Chartreuse;
        leds[NUM_LEDS / 2 + NUM_LEDS / 4] = CRGB::Purple;
      }
      rotateLEDs(1, 0);
      FastLED.show();
      break;
    default:
      break;
  }

}

void onePixelBackwardsAndForwards() {
  if (thisIsANewCue) {
    lednum = 0;
  }

  if (upwards == true) {
    lednum ++;
  } else {
    lednum --;
  }

  if (lednum >= NUM_LEDS - 1) {
    upwards = false;
  }

  if (lednum <= 0) {
    upwards = true;
  }

  setLEDs(CRGB::Black);
  leds[lednum] = CHSV(255, 0, 255);
  FastLED.show();
}

int middlepixel() {
  return (NUM_LEDS / 2);
}

int reflectPixel(int n) {
  return NUM_LEDS - n;
}


void twoPixelsReflected(int interval) {
  setLEDs(CRGB::Black);
  if (thisIsANewCue) {
    Serial.println("new");
    lednum = middlepixel() + 1;
  }
  lednum --;
  if (lednum < 0) {
    lednum =  middlepixel();
    delay(500);
  }
  leds[lednum] = CHSV(0, 0, 255);
  leds[reflectPixel(lednum) - 1] = CHSV(0, 0, 255);
  FastLED.show();
  delay(interval);
}

void wings() {
  setLEDs(CRGB::Black);
  if (thisIsANewCue) {
    lednum = middlepixel() + 1;
  }
  lednum --;
  if (lednum < 0) {
    lednum =  middlepixel();
  }

  for (int i = NUM_LEDS / 2; i >= lednum; i--) {
    leds[i] = CHSV(0, 0, 255);
  }

  int r = reflectPixel(lednum) - 1;
  for (int i = NUM_LEDS / 2; i <= r; i++) {
    leds[i] = CHSV(0, 0, 255);
  }

  FastLED.show();
    if (lednum == 0) {
    delay(1000);
  }

  delay(20);
}
