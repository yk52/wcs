/*
	Main class for the Vitameter.
*/

#include <Wire.h>
#include <Adafruit_CCS811.h>
#include <Adafruit_VEML6075.h>
// #include <ArduinoPedometer.h>
#include <vibration.h>
#include "C:\Users\Yumi\Desktop\wcs\config.h"

Adafruit_CCS811 ccs;
Adafruit_VEML6075 uv = Adafruit_VEML6075();
Vibration vib;
uint32_t millis = 0;

void setup() {
  // initialize the serial communications:
  Serial.begin(BAUDRATE);
  while (!Serial) { delay(10); }
  Serial.println("Vitameter has booted.");

  // Init I2C. Set GPIO4 and GPIO16 as SDA and SCL respectively.
  Wire.begin(4,16);
  
  vib.init();

  // UV init
  if (! uv.begin()) {
    Serial.println("Failed to communicate with VEML6075 sensor, check wiring?");
    while(1) { delay(100); }
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

}

void loop() {
  Serial.println("\n");
  Serial.print("UV Index reading: "); Serial.println(uv.readUVI());
  if(ccs.available()){
  float temp = ccs.calculateTemperature();
  if(!ccs.readData()){
    int co2 = ccs.geteCO2();
    Serial.print("CO2: ");
    Serial.print(co2);
    Serial.print("ppm, TVOC: ");
    Serial.print(ccs.getTVOC());
    Serial.print("ppb   Temp:");
    Serial.println(temp);
  }
  else{
    Serial.println("ERROR!");
    while(1);
  }
  if (co2 > CO2_LIMIT) {
    vib.on();
  }
  delay(1000);
  vib.off();
  }
}
