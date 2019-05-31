
//#include <Arduino.h>

#include <Adafruit_CCS811.h>
#include <BLE_wcs.h>
#include <Testbib.h>



Testbib bib;
Adafruit_CCS811 ada;
BLE_wcs ble;

void setup() {
  ble.init_server();
}

void loop() {


}
