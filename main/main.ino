
#include <BLE_wcs.h>
#include <Values.h>
#include <stdio.h>
#include <string>



BLE_wcs ble;
Values values;
std::string sent;
std::string processed = "prodef";

bool blue = false;
bool blueInit = false;
bool blueDeinit = false;


void setup() {
  Serial.begin(115200);

}

void loop() {
  /* 
   *  if a ble message from phone is received, process the message with values.processMessage
   *  Depending on return: set thresholds, get thresholds, send data, ...
   */
  if (Serial.available()) {
    Serial.println("serial.available");
    byte a = Serial.read();
    Serial.println(a);
    if (a == '1') {
      blue = true;
      blueInit = true;
    } else if (a == '0') {
      blue = false;
      blueDeinit = true;
    }
  }

  if (blue) {
    if (blueInit) {
      Serial.println("init called");
      blueInit = false;
      ble.init("Vitameter joey2");
    }
    Serial.println("blue on");
    Serial.print("message sent:   ");
    sent = ble.getMessage();
    Serial.println(sent.c_str());
    processed = values.processMessage(sent);
    Serial.print("message processed:   ");
    Serial.println(processed.c_str());  
    Serial.print("parameter:   ");
    Serial.println(values.parameter.c_str());    
    Serial.print("value is:   ");
    Serial.println(values._value);
    ble.write(processed);
    Serial.println("");
    Serial.println("");
  } else {
    if (blueDeinit) {
      Serial.println("deinit called");
      blueDeinit = false;
      ble.deinit();
    }
    Serial.println("blue off");
    Serial.println("");
    Serial.println("");
    ble.stopAdvertising();
  } 
  delay(2000);
}
