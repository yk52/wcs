


#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <BLE_wcs.h>
#include <stdio.h>

BLE_wcs ble;
Values values;

void setup() {
  Serial.begin(115200);
  ble.init("Vitameter joey");
}

void loop() {
  if (bleButton) {
    ble.init("Vitameter joey");
  }

  if (!bleButton) {
    ble.deinit();
  }
  /* 
   *  if a ble message from phone is received, process the message with values.processMessage
   *  Depending on return: set thresholds, get thresholds, send data, ...
   */
  if (ble.messageReceived()) {
    std::string txValue = values.processMessage(ble.getMessage());
    if (txValue.compare("DataRequest")) {         
      ble.deinit();                                                 // deinit BLE
      SerialBT.begin("Vitameter");
      SerialBT.print(data_package);
    }
    ble.write(txValue); 
    ble.messageReceived = false;
  }
  

}
