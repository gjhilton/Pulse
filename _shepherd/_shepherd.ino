/////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// DERWENT PULSE SHEPHERD CONTROLLER
//
//
////////////////////////////////////////////////////////////////////////////////////////////////

#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
#include "DebouncedButton.h"
#include "SmoothInput.h"

////////////////////////////////////////////////////////////////////////////////////////////////
// HARDWARE
////////////////////////////////////////////////////////////////////////////////////////////////

const int POTENTIOMETER_PIN =  A1;

const int RADIO_PIN = 8;
const int RESEND_BUTTON_PIN = 11;
const int PANIC_BUTTON_PIN = 12;
const int CLICKER_PIN = 13;

////////////////////////////////////////////////////////////////////////////////////////////////
// CONSTANT CONFIGURATION
////////////////////////////////////////////////////////////////////////////////////////////////

const bool USE_RADIO = true ;
const long BAUD_RATE = 115200;

const int STARTUP_DELAY_MILLIS = 1000;
const int PANIC_HOLD_DURATION = 5000;
const int BREATH_DURATION = 4000;
const int LOOP_DELAY = 1;

////////////////////////////////////////////////////////////////////////////////////////////////
// STATE VARIABLES
////////////////////////////////////////////////////////////////////////////////////////////////
int currentSceneNum = -1;
int currentCueNum = 0;

bool panicMode = false;
bool testPanic = false;
unsigned long panicButtonPressBeginTime = 0;

unsigned long lastBreathTime = 0;

Adafruit_7segment readout = Adafruit_7segment();
SmoothInput potentiometer = SmoothInput(POTENTIOMETER_PIN, sendParameter);
DebouncedButton clicker = DebouncedButton(CLICKER_PIN, nextCue);
DebouncedButton resend = DebouncedButton(RESEND_BUTTON_PIN, resendCue);

////////////////////////////////////////////////////////////////////////////////////////////////
// SHOW CUE PLOT
////////////////////////////////////////////////////////////////////////////////////////////////

// we want to share the same sourcecode as the costume sketch,
// but without lots of overhead of functions we never call
// so define no-ops that will pad the callBack functions of the
// scenes that are full of stuff in the other sketch
// these declarations must precede inclusion of ShowPlot.h
void sceneTest(int _) {};
void sceneWhiteFlashes(int _) {};
void sceneWhiteBreath(int _) {};
void sceneColour(int _) {};
void sceneMagnetometer(int _) {};
void sceneFinale(int _) {};

// the master listing of what happens when
#include "/path/to/shared/Scene.h"
#include "/path/to/shared/ShowPlot.h"
// the shared serial commands protocol
#include "/path/to/shared/SerialProtocol.h"

////////////////////////////////////////////////////////////////////////////////////////////////
// ARDUINO LIFECYCLE
////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  initReadout(); // do this before delay
  delay(STARTUP_DELAY_MILLIS);
  if (USE_RADIO) initRadio();
  initSerial();
  clicker.begin();
  resend.begin();
  initPanicButton();
}

void loop() {
  potentiometer.service();
  clicker.service();
  resend.service();
  servicePanicButton();
  serviceBreath();
  delay(LOOP_DELAY);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION: peripherals
////////////////////////////////////////////////////////////////////////////////////////////////

// PANIC BUTTON press and hold for 5 seconds to enter panic mode

void initPanicButton() {
  pinMode(PANIC_BUTTON_PIN, INPUT);
  digitalWrite(PANIC_BUTTON_PIN, HIGH);
}

void servicePanicButton() {
  int reading = digitalRead(PANIC_BUTTON_PIN);
  if (reading == LOW) {
    if (testPanic) {
      // panic button is still down
      if (millis() > (panicButtonPressBeginTime + PANIC_HOLD_DURATION)) goToPanicMode();
    } else {
      // panic button has just been pressed - start watching time
      panicButtonPressBeginTime = millis();
      testPanic = true;
    }
  } else {
    // no panic
    testPanic = false;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////
// LED 7-SEGMENT
////////////////////////////////////////////////////////////////////////////////////////////////

// 0b0000001  top horizontal
// 0b0000010  top rhs
// 0b0000100  bottom rhs
// 0b0001000  bottom horizontal
// 0b0010000  bottom lhs
// 0b0100000  top lhs
// 0b1000000  middle horiz

const short SEVENSEG_A = 0b1110111;
const short SEVENSEG_C = 0b0111001;
const short SEVENSEG_E = 0b1111001;
const short SEVENSEG_N = 0b0110111;
const short SEVENSEG_P = 0b1110011;
const short SEVENSEG_S = 0b1101101;
const short SEVENSEG_T = 0b1111000;
const short SEVENSEG_DASH = 0b1000000;

void initReadout() {
  readout.begin(0x70);
  helloReadout();
}

void sceneReadout() {
  if (currentSceneNum == 0) {
    // show tESt
    readout.writeDigitRaw(0, SEVENSEG_T);
    readout.writeDigitRaw(1, SEVENSEG_E);
    readout.writeDigitRaw(3, SEVENSEG_S);
    readout.writeDigitRaw(4, SEVENSEG_T);
    readout.drawColon(false);
  } else {
    // show S*:C*
    readout.writeDigitRaw(0, SEVENSEG_S);
    readout.writeDigitNum(1, currentSceneNum);
    readout.writeDigitRaw(3, SEVENSEG_C);
    readout.writeDigitNum(4, currentCueNum + 1); // NB the display is 1-based, not zero based
    readout.drawColon(true);
  }
  readout.writeDisplay();
}

void panicReadout() {
  // show PAnC
  readout.writeDigitRaw(0, SEVENSEG_P);
  readout.writeDigitRaw(1, SEVENSEG_A);
  readout.writeDigitRaw(3, SEVENSEG_N);
  readout.writeDigitRaw(4, SEVENSEG_C);
  readout.drawColon(false);
  readout.writeDisplay();
}

void helloReadout() {
  // show --:--
  readout.writeDigitRaw(0, SEVENSEG_DASH);
  readout.writeDigitRaw(1, SEVENSEG_DASH);
  readout.writeDigitRaw(3, SEVENSEG_DASH);
  readout.writeDigitRaw(4, SEVENSEG_DASH);
  readout.drawColon(true);
  readout.writeDisplay();
}

////////////////////////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION: cursor in the show plot
////////////////////////////////////////////////////////////////////////////////////////////////

void nextCue() {
  if (currentCueNum + 1 < SHOW_PLOT[currentSceneNum].numCues) {
    // if there's another cue in the scene go to that
    currentCueNum++;
  } else {
    // we've finished the scene so go to the next scene
    currentSceneNum++;
    currentCueNum = 0;
  }
  // if we've wlked off the end, loop back to the beginning
  if (currentSceneNum >= NUM_SCENES_IN_SHOW_PLOT) {
    currentSceneNum = 0;
    currentCueNum = 0;
  }
  sceneReadout();
  sendCue(currentSceneNum, currentCueNum);
}

void resendCue() {
  sendCue(currentSceneNum, currentCueNum);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION: breath
////////////////////////////////////////////////////////////////////////////////////////////////

void serviceBreath() {
  unsigned long now = millis();
  if (now > (lastBreathTime + BREATH_DURATION)) {
    sendBreath(BREATH_DURATION);
    lastBreathTime = now;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION: communication
////////////////////////////////////////////////////////////////////////////////////////////////

void initSerial() {
  Serial.begin(BAUD_RATE);
  Serial.flush();
}

void initRadio() {
  pinMode(RADIO_PIN, OUTPUT);
  digitalWrite(RADIO_PIN, HIGH);
}

void sendCue(int scene, int cue) {
  Serial.print(COMMAND_DELIMITER_SCENE);
  Serial.print(scene);
  Serial.print(COMMAND_DELIMITER_CUE);
  Serial.println(cue);
}

void sendBreath(int duration) {
  // for now duration is ignored
  Serial.println(COMMAND_DELIMITER_BREATH);
}

void sendParameter() {
  Serial.print(COMMAND_DELIMITER_PARAMETER);
  Serial.println(potentiometer.getValue());
}

void goToPanicMode() {
  panicMode = true;
  panicReadout();
  Serial.print(COMMAND_DELIMITER_PANIC);
}
