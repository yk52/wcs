/*
  Test prepare Data function
*/


#include <BluetoothSerial.h>
#include <BLE_wcs.h>
#include <Values.h>
#include "C:\Users\Yumi\Desktop\wcs\config.h"




BluetoothSerial SerialBT;
// BLE_wcs ble;

Values values;



void setup() {
   Serial.begin(BAUDRATE);

}

void loop() {
 
      Serial.println(ms);
      Serial.print("AQ: ");
      Serial.println(values.getLastCO2());
      Serial.print("UV: ");
      Serial.println(values.getLastUVI());
      Serial.print("Temp: ");
      Serial.println(values.getLastTemp());
      Serial.println();
      





}
