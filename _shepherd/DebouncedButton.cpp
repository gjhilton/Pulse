#include "DebouncedButton.h"
#include "Arduino.h"

DebouncedButton::DebouncedButton(short _pin, ButtonCallback _callback) {
  pin = _pin;
  callback = _callback;
  pressedState = LOW;
  lastPressedState = LOW;
  previousDebounceMillis = 0;
}

void DebouncedButton::begin() {
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH);
}

void DebouncedButton::service() {
  int reading = digitalRead(pin);
  if (reading != lastPressedState) {
    previousDebounceMillis = millis();
  }
  if ((millis() - previousDebounceMillis) > BUTTON_DEBOUNCE_TIME_MS) {
    if (reading != pressedState) {
      pressedState = reading;
      if (pressedState == HIGH) {
        callback();
      }
    }
  }
  lastPressedState = reading;
}
