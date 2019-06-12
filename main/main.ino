/*
	Main class for the Vitameter.
*/

#include <Wire.h>
#include <BluetoothSerial.h>
#include <Adafruit_CCS811.h>
#include <Adafruit_VEML6075.h>
#include <Adafruit_ADXL335.h>
#include <InterfaceOut.h>
#include <Timer.h>
#include "C:\Users\Yumi\Desktop\wcs\config.h"

Adafruit_CCS811 ccs;
Adafruit_VEML6075 uv = Adafruit_VEML6075();
Adafruit_ADXL335 pedo;
BluetoothSerial SerialBT;
InterfaceOut vib(VIBRATION_PIN);
InterfaceOut led(LEDRED_PIN);
Timer timer;

bool error = 0;

uint32_t ms = 0;
uint32_t pedoTimeout = 0;
uint32_t uvTimeout = 0;
uint32_t airTimeout = 0;
int c = 0;

uint32_t showTimeout = 0;

int x = 0;
int oldX = 0;
uint16_t co2[CO2_STORAGE_SIZE];  // in ppm
uint16_t co2_idx = 0;
uint16_t voc[VOC_STORAGE_SIZE]; // in ppb
uint16_t voc_idx = 0;
uint8_t uvi[UVI_STORAGE_SIZE];   // UV index
uint16_t uvi_idx = 0;
float temp[TEMP_STORAGE_SIZE]; // in degrees
uint16_t temp_idx = 0;

void setup() {
  // initialize the serial communications:
  SerialBT.begin("Vitameter");
  Serial.begin(BAUDRATE);
  while (!Serial) {
    delay(10);
  }
  Serial.println("Vitameter has booted.");

  // Init I2C. Set GPIO4 and GPIO16 as SDA and SCL respectively.
  Wire.begin(SDA_PIN, SCL_PIN);

  // UV 
  if (!uv.begin()) {
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
  float t = ccs.calculateTemperature();
  ccs.setTempOffset(t - 25.0);
  
  pedo.calibrate();
  
}

void loop() {
  if (error) {
      Serial.println("ERROR!");
      led.on();
      while(1);
  }

  ms = timer.getMillis();

  if (ms > pedoTimeout) {
    x = pedo.getPedo(); //get the no. of steps
    if (pedo.flag) {
      Serial.println(x);
      pedoTimeout += WAIT_AFTER_STEP;
      pedo.flag = 0;
    }
    else {
      pedoTimeout += PEDO_FREQ;
    }
  }

  if ((ms > airTimeout) && ccs.available()) {
    if (!ccs.readData()) {
      co2[co2_idx++] = ccs.geteCO2();
      voc[voc_idx++] = ccs.getTVOC();
      temp[temp_idx++] = ccs.calculateTemperature();
    } else {
      error = 1;
    }
    airTimeout += AQ_FREQ;
  }
  if (ms > uvTimeout) {
    uvi[uvi_idx++] = uv.readUVI();
  }

  if (ms > showTimeout) {
    SerialBT.println(pedo.steps);
    Serial.println(pedo.steps);

    Serial.print("AQ: ");
    Serial.println(co2[co2_idx-1]);
    Serial.print("UV: ");
    Serial.println(uvi[uvi_idx-1]);
    Serial.print("Steps: ");
    Serial.println(pedo.steps);
    Serial.println();
    showTimeout += 1000;
  }
}
