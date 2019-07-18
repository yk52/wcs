#ifndef INTERFACEOUT_H_
#define INTERFACEOUT_H_

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h>
#include <Config.h>



class InterfaceOut {
	public:
		//constructors
		InterfaceOut(int pinNr) {
			this->pin = pinNr;
			pinMode(pinNr, OUTPUT);
			digitalWrite(pinNr, LOW);
		};
		~InterfaceOut(void) {};

		uint8_t pos;
		uint8_t repetitions;
		uint32_t schedule[10] = {0};

		void on(void);
		void off(void);
		void toggle(void);
		void rhythm(uint32_t duration, uint32_t pause, uint8_t repetitions);

	private:
		int pin;
};

#endif /* INTERFACEOUT_H_ */
