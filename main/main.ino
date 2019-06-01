/*
	Main class for the Vitameter.
*/

#include <Wire.h>
#include <Adafruit_CCS811.h>
#include <Adafruit_VEML6075.h>
// #include <ArduinoPedometer.h>
#include <InterfaceOut.h>
#include <Timer.h>
#include "C:\Users\Yumi\Desktop\wcs\config.h"

Adafruit_CCS811 ccs;
Adafruit_VEML6075 uv = Adafruit_VEML6075();
InterfaceOut vib(VIBRATION_PIN);
InterfaceOut led(LEDRED_PIN);
Timer timer;
uint32_t ms = 0;
uint32_t ticks = 0;
int co2 = 0;  // in ppm
int voc = 0; // in ppb
int uvi = 0;   // UV index
int temp = 0; // in degrees

void setup() {
  // initialize the serial communications:
  Serial.begin(BAUDRATE);
  while (!Serial) { delay(10); }
  Serial.println("Vitameter has booted.");

  // Init I2C. Set GPIO4 and GPIO16 as SDA and SCL respectively.
  Wire.begin(4,16);

  // UV init
  if (! uv.begin()) {
    Serial.println("Failed to communicate with VEML6075 sensor, check wiring?");
    while(1);
  }
  Serial.println("Found VEML6075 sensor");


  // Air Quality init
  if(!ccs.begin()){
    Serial.println("Failed to start Air Quality sensor! Please check your wiring.");
    while(1);
  }
  Serial.println("Found CCS811 sensor");
  
  //calibrate temperature sensor on CCS811
  while(!ccs.available());
  float temp = ccs.calculateTemperature();
  ccs.setTempOffset(temp - 25.0);

  led.on();
}

void loop() {
  ms = timer.getMillis();
  if (ms % PEDO_FREQ == 0) {
    // get Pedo
  } else if (ms % AQ_FREQ == 0 && ccs.available()) {
    if (!ccs.readData()) {
      co2 = ccs.geteCO2();
      voc = ccs.getTVOC();
      temp = ccs.calculateTemperature();
    } else {
      Serial.println("ERROR!");
      while(1);
    }
  } else if (ms % UV_FREQ == 0) {
    uvi = uv.readUVI();
  }
}
