/*
	Main class for the Vitameter.
*/

#include <Wire.h>
#include <Adafruit_CCS811.h>
#include <Adafruit_VEML6075.h>
#include <Adafruit_ADXL335.h>
#include <InterfaceOut.h>
#include <Timer.h>
#include "C:\Users\Yumi\Desktop\wcs\config.h"

Adafruit_CCS811 ccs;
Adafruit_VEML6075 uv = Adafruit_VEML6075();
Adafruit_ADXL335 pedo;
InterfaceOut vib(VIBRATION_PIN);
InterfaceOut led(LEDRED_PIN);
Timer timer;

uint32_t ms = 0;
uint32_t pedoTimeout = 0;
uint32_t uvTimeout = 0;
uint32_t airTimeout = 0;

int x = 0;
int oldX = 0;
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
  
  calibratePedo();
  
}

void calibratePedo() {
  pedo.setAverage(); // perform calibration to get x,y,z parameters.
  Serial.println("Leave the pedometer on the table. Then press Button.");
  while (digitalRead(BUTTON_PIN) == HIGH) {}
  delay(1000);
  for (int n=0; n < 5; n++);
  {
    pedo.calibrate();
  }
  Serial.println("Turn towards y-Axis. Then press Button.");
  while (digitalRead(BUTTON_PIN) == HIGH) {}
  delay(1000);
  for (int n=0; n < 5; n++);
  {
    pedo.calibrate();
  }
  Serial.println("Turn towards x-Axis. Then press Button.");
  while (digitalRead(BUTTON_PIN) == HIGH) {}
  delay(1000);
  for (int n=0; n < 5; n++);
  {
    pedo.calibrate();
  }
  Serial.println("Calibration Successful!");
  Serial.print("Raw Ranges: X: ");
  Serial.print(pedo.xRawMin);
  Serial.print("-");
  Serial.print(pedo.xRawMax);
  
  Serial.print(", Y: ");
  Serial.print(pedo.yRawMin);
  Serial.print("-");
  Serial.print(pedo.yRawMax);
  
  Serial.print(", Z: ");
  Serial.print(pedo.zRawMin);
  Serial.print("-");
  Serial.print(pedo.zRawMax);
  Serial.println();
}

void loop() {
  ms = timer.getMillis();
  if (ms > pedoTimeout) {
    x = pedo.getPedo(); //get the no. of steps
    if (x != oldX) {
      Serial.println(x);
      oldX = x;
      pedoTimeout += PEDO_FREQ;
    }
  }
  if (ms > airTimeout && ccs.available()) {
    if (!ccs.readData()) {
      co2[co2_idx++] = ccs.geteCO2();
      voc[voc_idx++] = ccs.getTVOC();
      temp[temp_idx++] = ccs.calculateTemperature();
    } else {
      Serial.println("ERROR!");
      while (1);
    }
    airTimeout += AQ_FREQ;
  }
  if (ms > uvTimeout) {
    uvi[uvi_idx++] = uv.readUVI();
  }
}
