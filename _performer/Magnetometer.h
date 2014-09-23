#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

#ifndef __lp_magnetometer__
#define __lp_magnetometer__

class Magnetometer {

  public:

    Magnetometer();

    void begin();
    float read();

  private:
    Adafruit_HMC5883_Unified mag;
    boolean available;
};

#endif
