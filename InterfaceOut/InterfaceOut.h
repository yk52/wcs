#ifndef INTERFACEOUT_H_
#define INTERFACEOUT_H_

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h>
#include "C:\Users\Yumi\Desktop\wcs\config.h"



class InterfaceOut {
	public:
		//constructors
		InterfaceOut(int pinNr) {
			this->pin = pinNr;
			pinMode(pinNr, OUTPUT);
			digitalWrite(pinNr, LOW);
		};
		~InterfaceOut(void) {};


		void on(void);
		void off(void);
		void toggle(void);

	private:
		int pin;
};

#endif /* INTERFACEOUT_H_ */
