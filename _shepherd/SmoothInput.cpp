#include "SmoothInput.h"
#include "Arduino.h"

SmoothInput::SmoothInput(short _pin, NewValueCallback _callback) {
  pin = _pin;
  callback = _callback;
  for (int thisReading = 0; thisReading < SMOOTH_INPUT_BUFFER_SIZE; thisReading++) {
    readings[thisReading] = 0;
  }
  stable = false;
}

void SmoothInput::service() {
  total = total - readings[currentIndex];
  readings[currentIndex] = analogRead(pin);
  total = total + readings[currentIndex];
  currentIndex = currentIndex + 1;
  if (currentIndex >= SMOOTH_INPUT_BUFFER_SIZE){
    if (!stable) {
      stable = true; // we've filled the buffer, so results should now be stable   
    }
    currentIndex = 0;
  }
  average = total / SMOOTH_INPUT_BUFFER_SIZE;
  if (abs(average - previousAverage) > CHANGE_THRESHOLD){
    // we have a new average reading
    if (stable) callback();
    previousAverage = average;
  }
}

int SmoothInput::getValue() {
  return average;
}
