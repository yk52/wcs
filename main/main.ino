#include "BluetoothSerial.h"
#include <BLE_wcs.h>
#include <DataTest.h>

BLE_wcs ble;
 
BluetoothSerial SerialBT;
DataTest dataTest;
 
void setup() {
  SerialBT.begin("Vitameter");
  Serial.begin(115200);
}

void loop() {
  
  for(int i = 0; i < 10; i++) {
    SerialBT.println("bluetooth on");
    Serial.println("bluetooth on");
    delay(1000);
  }

  for(int i = 0; i < 10; i++) {
    Serial.println("bluetooth off");
    delay(1000);
  }
}
