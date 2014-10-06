/////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// DERWENT PULSE PERFORMER COSTUME
//
//
////////////////////////////////////////////////////////////////////////////////////////////////

#define USE_MAGNETOMETER;

#include <FastLED.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

#include "Magnetometer.h"
#include "envelope.h"
#include "interpolation.h"

// Forward declarations: must precede inclusion of ShowPlot.h
void sceneTest(int cue);
void sceneWhiteFlashes(int cue);
void sceneWhiteBreath(int cue);
void sceneChase(int cue);
void sceneColour(int cue);
void sceneMagnetometer(int cue);

#include "/path/to/DerwentPulse/shared/ArrayLength.h"
#include "/path/to/DerwentPulse/shared/Scene.h"
#include "/path/to/DerwentPulse/shared/SerialProtocol.h"
#include "/path/to/DerwentPulse/shared/ShowPlot.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
// HARDWARE CONFIGURATION
////////////////////////////////////////////////////////////////////////////////////////////////

const short NUM_LEDS = 48;
const short DATA_PIN = 11;
const short CLOCK_PIN = 13;
const short RADIO_PIN = 8;

const short HERO_LED_RED_PIN = 6;
const short HERO_LED_GREEN_PIN = 5;
const short HERO_LED_BLUE_PIN = 3;

const short EMPTY_ANALOGUE_PIN = A0;

/////////////////////////////////////////////////////////////////////////////////////////////////
// CONSTANT CONFIGURATION
////////////////////////////////////////////////////////////////////////////////////////////////

// #define SERIAL_DEBUG; // uncomment for serial status reports - but nb this will break radio when there are multiple clients :(

const long BAUD_RATE = 115200;

const short STARTUP_DELAY = 1000;
const short LOOP_DELAY = 20;

const short BREATH_LENGTH_MS = 6000;
const int MIN_BREATH_BRIGHTNESS = 20;

/////////////////////////////////////////////////////////////////////////////////////////////////
// STATE VARIABLES
////////////////////////////////////////////////////////////////////////////////////////////////

Magnetometer compass = Magnetometer();

String commandString = "";
boolean commandStringComplete = false;

boolean thisIsANewCue = false;

CRGB leds[NUM_LEDS];

boolean stateOfPanic = false; // once this flag is set, we're no longer in radio command mode
unsigned long lastBreathMillis = 0;

boolean offline = true;
unsigned long lastHeartbeatMillis = 0;

int currentScene;
int currentCue;
int lastParameter;

/////////////////////////////////////////////////////////////////////////////////////////////////
// ARDUINO LIFECYCLE
////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  initHeroLED(); // do this first just to shut it off
  // a brief wait so board isn't locked up
  delay(STARTUP_DELAY);
  randomSeed(analogRead(EMPTY_ANALOGUE_PIN));
  // init peripherals
  initLEDTape();
  initSerial();
  compass.begin();
  // init state
  breath();
  currentScene = 0;
  currentCue = 0;
  lastParameter = 0;

#ifdef SERIAL_DEBUG
  Serial.println("setup --> ready");
#endif

}

void loop() {
  if (!stateOfPanic) {
    processCommands();
  }
  loopScene();
  delay(LOOP_DELAY);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Hero LED
////////////////////////////////////////////////////////////////////////////////////////////////

void initHeroLED() {
  pinMode(HERO_LED_RED_PIN, OUTPUT);
  pinMode(HERO_LED_GREEN_PIN, OUTPUT);
  pinMode(HERO_LED_BLUE_PIN, OUTPUT);
  // because of the unused GPS unit on the boards, set to LOW reclaim pin
  digitalWrite(HERO_LED_RED_PIN, LOW);
  digitalWrite(HERO_LED_GREEN_PIN, LOW);
  digitalWrite(HERO_LED_BLUE_PIN, LOW);
  heroLED(CRGB::Black);
}

void heroLED(CRGB colour) {
  analogWrite(HERO_LED_RED_PIN, colour.r);
  analogWrite(HERO_LED_GREEN_PIN, colour.g);
  analogWrite(HERO_LED_BLUE_PIN, colour.b);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// LED Strip
////////////////////////////////////////////////////////////////////////////////////////////////

void initLEDTape() {
  FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
  setLEDs(CRGB::Black);
}

void rotateLEDs(int rotate_by, bool clockwise) {
  rotateArray(rotate_by, leds, NUM_LEDS, clockwise);
}

void rotateArray(int rotate_by, CRGB array[], int array_length, bool clockwise) {
  CRGB replacement[rotate_by];
  if (clockwise) {
    // {1,2,3,4,5,6,7,8,9} rotate_by 2 => {8,9,1,2,3,4,5,6,7}
    for (int i = 0; i < rotate_by; i++) {
      replacement[i] = array[array_length - (rotate_by - i)];
    }
    for (int i = array_length; i >= rotate_by; i-- ) {
      array[i] = array [i - rotate_by];
    }
    for (int i = 0; i < rotate_by; i++) {
      array[i] = replacement[i];
    }
  } else {
    // {1,2,3,4,5,6,7,8,9} rotate_by 2 => {3,4,5,6,7,8,9,1,2}
    for (int i = 0; i < rotate_by; i++) {
      replacement[i] = array[i];
    }
    for (int i = 0; i < array_length - rotate_by; i++) {
      array[i] = array[i + rotate_by];
    }
    for (int i = 0; i < rotate_by; i++) {
      array[array_length - (rotate_by - i)] = replacement[i];
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////
// SERIAL COMMUNICATION
////////////////////////////////////////////////////////////////////////////////////////////////

void initSerial() {
  pinMode(RADIO_PIN, OUTPUT);
  digitalWrite(RADIO_PIN, HIGH);
  Serial.begin(BAUD_RATE);
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (!commandStringComplete) {
      commandString += inChar;
      if (inChar == COMMAND_DELIMITER_END) {
        commandStringComplete = true;
      }
    } else {
      // nothing - discard the additional input
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////
// COMMANDS
////////////////////////////////////////////////////////////////////////////////////////////////

void processCommands() {
  if (commandStringComplete) {
    parseCommand(commandString );
    commandString = "";
    commandStringComplete = false;
  }
}

void parseCommand(String str) {
  switch (str.charAt(0)) {
    case COMMAND_DELIMITER_BREATH:
      breath();
      break;
    case COMMAND_DELIMITER_HEARTBEAT:
      heartbeat();
      break;
    case COMMAND_DELIMITER_PANIC:
      panic();
      break;
    case COMMAND_DELIMITER_PARAMETER: {
        int v = str.substring(1).toInt();
        newParameter(v);
        break;
      }
    case COMMAND_DELIMITER_SCENE: {
        // we may have a scene command
        int cutPoint = str.indexOf(COMMAND_DELIMITER_CUE);
        int len = str.length();
        // cutPoint of	1 is invalid because we need s[number]c
        // cutPoint of (length -2) is invalid because we need c[number]\n
        if ((cutPoint > 1) && (cutPoint < len - 2)) {
          int s = str.substring(1, cutPoint).toInt(); // first character is COMMAND_DELIMITER_SCENE
          int c = str.substring(cutPoint + 1, len - 1).toInt(); // trim off COMMAND_DELIMITER_END at end
          play(s, c);
          break;
        }
      }
      // invalid input, so fall through to default!
    default:
      invalidInput();
      break;
  }
}

void play(int scene, int cue) {
  if ((scene >= NUM_SCENES_IN_SHOW_PLOT) || (scene < 0)) {
    invalidInput();
    return;
  }
  if ((scene != currentScene) || (cue != currentCue)){
    goOnline();
    thisIsANewCue = true;
    currentScene = scene;
    currentCue = cue;
  }
}

void invalidInput() {

#ifdef SERIAL_DEBUG
  Serial.println("invalid input");
#endif

  0; // NoOp
}

void newParameter(int value) {
  lastParameter = value;
  Serial.println(lastNormalisedParameter());
}

float lastNormalisedParameter() {
  return lastParameter / 1024.f;
}

void loopScene() {
  if (!stateOfPanic) {
    SHOW_PLOT[currentScene].callBack(currentCue);
    thisIsANewCue = false;
  } else {
    scenePanicLoop();
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////
// HEARTBEAT
////////////////////////////////////////////////////////////////////////////////////////////////

void heartbeat() {
  lastHeartbeatMillis = millis();
}

void goOffline(){
  offline = true;
}

void goOnline(){
  offline = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// BREATH
////////////////////////////////////////////////////////////////////////////////////////////////

void breath() {
  lastBreathMillis = millis();
}

unsigned long millisSinceBreath() {
  unsigned long m = millis() - lastBreathMillis;
  return m;
}

float getBreathPhase(){
  unsigned long m = millisSinceBreath();
  float b;
  if (m <= 0) {
    b = 0; 
  } else if (m > BREATH_LENGTH_MS) {
    b = 1;
  } else {
   b = float(millisSinceBreath())/BREATH_LENGTH_MS;
  }
  return b;
}

void setLEDs(CRGB c) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = c;
  }
  showLEDs();
}

void showLEDs() {
  FastLED.show();
}
