# wcs
Wearable Computing Systems Project: Code for the microcontroller.

The final submission branch is "vitameterAbgabe".
Clone said branch for a working version.

In order for the code to compile, add all files/libraries on the branch to the Arduino library manually.
Also, change the path to the config.h file according to your folder environment.
(Arduino does not allow a generalized way to include files and libraries, such as "../config.h". A specific path must be named)

The path to config must be specified in:
Adafruit_ADXL335.cpp
Adafruit_CCS811.cpp
BLE_wcs.cpp
InterfaceOut.cpp
Values.cpp
main.ino
