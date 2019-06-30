
#include <BLE_wcs.h>
#include <Values.h>
#include <stdio.h>
#include <string>



BLE_wcs ble;
Values values;
std::string sent;
std::string processed = "prodef";
  
void setup() {
  Serial.begin(115200);
  ble.init("Vitameter joey");
}

void loop() {
  /* 
   *  if a ble message from phone is received, process the message with values.processMessage
   *  Depending on return: set thresholds, get thresholds, send data, ...
   */
  //Serial.println(values.co2Thresh);
  //if (ble.messageReceived) {
  if (true) {
    Serial.println("message received");
    sent = ble.getMessage();
    Serial.println(sent.c_str());
    processed = values.processMessage(sent);

    Serial.print("processed: ");
    Serial.println(processed.c_str());
    
    Serial.print("parameter is ");
    Serial.println(values.parameter.c_str());
    
    Serial.print("_value is ");
    Serial.println(values._value);
    
    Serial.print("testThresh is ");
    Serial.println(values.testThresh);
    Serial.println("");
    Serial.println("");

    ble.write(processed);
    
    //txValue = values.processMessage(ble.getMessage());
    //if (txValue.compare("DataRequest")) {         
      // send data
    //}
    //Serial.println(txValue.c_str());
    //ble.write(txValue); 
   // ble.messageReceived = false;
  }
  delay(2000);
}
