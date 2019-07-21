

#include "BluetoothSerial.h"
#include <BLE_wcs.h>
#include <DataTest.h>
#include <Values.h>

BLE_wcs ble;
Values values;
BluetoothSerial serialBT;
DataTest dataTest;
 
void setup() {
  serialBT.begin("Vita");
  Serial.begin(115200);
}

void loop() {
  
  //for(int i = 0; i < 10; i++) {

    Serial.println("bluetooth on");
    
    serialBT.println(values.prepareAllData().c_str());
    serialBT.println(values.prepareCO2Data().c_str());
    serialBT.println(values.prepareVOCData().c_str());
    serialBT.println(values.prepareTempData().c_str());
    serialBT.println(values.prepareUVIData().c_str());
    
    serialBT.println("hello");
    
    delay(2000);
  //}

}
