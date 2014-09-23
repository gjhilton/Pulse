#include "Magnetometer.h"

Magnetometer::Magnetometer() {
  available = false;
  mag = Adafruit_HMC5883_Unified(12345); // hardcoded UID
}

void Magnetometer::begin() {
  if (mag.begin()) {
    available = true;
  };
}

float Magnetometer::read() {
  if (available) {
    sensors_event_t event;
    mag.getEvent(&event);
    float heading = atan2(event.magnetic.z, event.magnetic.x);
    if (heading < 0) heading += 2 * PI;
    float headingDegrees = heading * 180 / M_PI; // TODO unnecessary - just easier to debug with degrees
    float normalisedHeading = headingDegrees / 360; //UGH quick and dirty - TODO tidy
    return normalisedHeading;
  } else {
    return 1;
  }
}
