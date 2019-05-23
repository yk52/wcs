/*
	Main class for the Vitameter.
*/

#include <Wire.h>
#include <vibration.h>
#include "C:\Users\Yumi\Desktop\wcs\config.h"

// #include "C:\Users\Yumi\Desktop\wcs\Scheduler\scheduler.h"

Vibration vib;

void setup() {
  // initialize the serial communications:
  Serial.begin(BAUDRATE);
  vib.init();
}

void loop() {
  vib.on();
  delay(200);
  vib.off();
  delay(200);
}
