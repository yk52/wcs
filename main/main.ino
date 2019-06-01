/*
	Main class for the Vitameter.
*/

#include <Wire.h>
#include <Adafruit_CCS811.h>
#include <Adafruit_VEML6075.h>
#include <ArduinoPedometer.h>
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
uint16_t co2[CO2_STORAGE_SIZE];  // in ppm
uint16_t co2_idx = 0;
uint16_t voc[VOC_STORAGE_SIZE]; // in ppb
uint16_t voc_idx = 0;
uint8_t uvi[UVI_STORAGE_SIZE];   // UV index
uint16_t uvi_idx = 0;
double temp[TEMP_STORAGE_SIZE]; // in degrees
uint16_t temp_idx = 0;

void setup() {
  // initialize the serial communications:
  Serial.begin(BAUDRATE);
  while (!Serial) {
    delay(10);
  }
  Serial.println("Vitameter has booted.");

  // Init I2C. Set GPIO4 and GPIO16 as SDA and SCL respectively.
  Wire.begin(4, 16);

  // UV init
  if (! uv.begin()) {
    Serial.println("Failed to communicate with VEML6075 UV sensor! Please check your wiring.");
    while (1);
  }
  Serial.println("Found VEML6075 (UV) sensor");


  // Air Quality init
  if (!ccs.begin()) {
    Serial.println("Failed to start Air Quality sensor! Please check your wiring.");
    while (1);
  }
  Serial.println("Found CCS811 (Air Quality) sensor");

  //calibrate temperature sensor on CCS811
  while (!ccs.available());
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
      co2[co2_idx++] = ccs.geteCO2();
      voc[voc_idx++] = ccs.getTVOC();
      temp[temp_idx++] = ccs.calculateTemperature();
    } else {
      Serial.println("ERROR!");
      while (1);
    }
  } else if (ms % UV_FREQ == 0) {
    uvi[uvi_idx++] = uv.readUVI();
  }
}
