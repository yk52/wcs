#include "BluetoothSerial.h"
#include <BLE_wcs.h>
#include <Values.h>
#include <stdio.h>
#include <string>

/*************************************************
 * type via console:
 * 0 for ble init
 * 1 for ble deinit
 * 2 for btserial init
 * 3 for btserial deinit
 */

Values values;


// ----------------- blue low energy -------------------
BLE_wcs ble;
std::string sent;
std::string processed = "default";
bool blueLE_on = false;
bool blueLE_init = false;
bool blueLE_deinit = false;
// ------------------------------------------------------

// ---------------- blue serial ------------------------
BluetoothSerial serialBT;
bool blueSerial_on = false;
bool blueSerial_init = false;
bool blueSerial_deinit = false;
// ------------------------------------------------------

// --------------- display message function ------------
void displayBleMessage() {
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
}
// ------------------------------------------------------


/***********************************************************************+
 *               SETUP
 *************************************************************************/
void setup() {
  Serial.begin(115200);
  serialBT.begin("Vitameter blue serial");
}



/***********************************************************************+
 *               LOOP
 *************************************************************************/

void loop() {

  /********************************************************************************
   *                       console input logic                                    */
     
  if (Serial.available()) {
    byte a = Serial.read();
    if (a == '0') {
      blueLE_on = true;
      blueLE_init = true;
    } else if (a == '1') {
      blueLE_on = false;
      blueLE_deinit = true;
    } else if (a == '2') {
      blueSerial_on = true;
      blueSerial_init = true;
    } else if (a == '3') {
      blueSerial_on = false;
      blueSerial_deinit = true;
    }
  }

  /********************************************************************************
   *                       blue low energy                                               */
  if (blueLE_on) {
    if (blueLE_init) {
      Serial.println("ble init called");
      blueLE_init = false;
      ble.init("Vitameter ble");
    }
    displayBleMessage();
  } else {
    if (blueLE_deinit) {
      Serial.println("blue low energy deinit called");
      blueLE_deinit = false;
      serialBT.end();                                 // does reboot and maybe this kills also ble?!
    }
    Serial.println("blue low energy off");
    Serial.println("");
  }

  /*************************************************************************
   *            blue serial
   */
  if (blueSerial_on) {
    if (blueSerial_init) {
      Serial.println("blue serial init called");
      blueSerial_init = false;
      serialBT.begin("Vitameter blue serial");
    }
    Serial.println("serial bluetooth on");
    serialBT.println(values.prepareAllData().c_str());
    serialBT.println(values.prepareCO2Data().c_str());
    serialBT.println(values.prepareVOCData().c_str());
    serialBT.println(values.prepareTempData().c_str());
    serialBT.println(values.prepareUVIData().c_str());
  } else {
    if (blueSerial_deinit) {
      Serial.println("blue serial deinit called");
      blueSerial_deinit = false;
      serialBT.end();
    }
    Serial.println("serial blue off");
    Serial.println("");
    Serial.println("");
  }
   
  delay(2000);
}
