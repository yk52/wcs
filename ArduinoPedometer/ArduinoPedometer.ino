/************* Arduino Pedometer with HC-05 ******************
  This project collects data from the ADXL-335 accelerometer.
  Arduino processes it and calculates the "No. of Steps" taken
  from the time of power up. the data is encoded in Json string format
  and sent serially using a BT module HC-05.
  ---------------------------------------------------------------------
  created on 16 Dec 2015
  by Abhay Shankar Bharadwaj
  (abhaysbharadwaj@gmail.com)
  (facebook.com/abhaysbharadwaja)
  ---------------------------------------------------------------------
  credits go to Benoit Blanchon for the ArduinoJson library
  and jatinbatra (http://www.instructables.com/member/jatinbatra/)
  who created the original pedometer code.
  ---------------------------------------------------------------------
  This project code is in the public domain.
  It is given as is, and any modifications can be done
  as an open sorce standard. credits to be updated wherever
  and whenever necessary.
  ---------------------------------------------------------------------
  ----------------------- connection details --------------------------
  ADXL335 accelerometer & Arduino nano:
  Vcc --> +5v
  X --> A5
  Y --> A6
  Z --> A7
  Gnd --> gnd
  BT HC05 Tx to software Rx (D11)
  BT HC05 Rx to software Tx (D10)
  you can change the pind in .h file
  ----------------------------------------------------------------------
  you must include the "ArduinoJson" and "SoftwareSerial"
  libraries in arduino's library directory
  (C:\Program Files (x86)\Arduino\libraries)
  (or ...\Arduino\libraries)
  you can download the libraries from:
  ArduinoJson: https://github.com/bblanchon/ArduinoJson
  SoftwareSerial: https://github.com/arduino/Arduino/tree/master/hardware/arduino/avr/libraries/SoftwareSerial
**********************************************************/
 
#include <Adafruit_ADXL335.h>
#include <Timer.h>
#include <InterfaceOut.h>

ArduinoPedometer pedo; //create an instacne called "pedo" for the class "ArduinoPedometer"
int x;
int z;
Timer timer;
int oldX = 0;
bool calibrated = false;
InterfaceOut led(LEDRED_PIN);

void setup()
{
  Serial.begin(BAUDRATE);
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

void loop()
{
  if (digitalRead(BUTTON_PIN) == LOW)
  {
    pedo.calibrate();
  }
  x = pedo.getPedo(); //get the no. of steps
  if (x != oldX) {
    Serial.println(x);
    oldX = x;
  }
  delay(20);
}
