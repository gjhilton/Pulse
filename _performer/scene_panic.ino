void panic() {
  stateOfPanic = true; // this is permanent: panic is forever
  for (int i = 0; i < NUM_LEDS; i++) {
    int hue = (255.f/NUM_LEDS) * i;
    leds[i] = CHSV(hue, 255, 255);
  }
  FastLED.show();
}

void scenePanicLoop() {
  rotateLEDs(1, 0);
  FastLED.show();
}
