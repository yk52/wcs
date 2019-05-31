#ifndef VIBRATION_H_
#define VIBRATION_H_

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h>
#include <time.h>
#include "C:\Users\Yumi\Desktop\wcs\config.h"



class Vibration {
	public:
		//constructors
		Vibration(void) {};
		~Vibration(void) {};

		bool flag;
		uint8_t pos;
		uint8_t repetitions;
		uint32_t schedule[10] = {0};

		void init(void);
		uint32_t getMillis(void);
		uint32_t getTicks(void);
		void on(void);
		void off(void);
		void toggle(void);
		void check(void);
		void rhythm(uint32_t duration, uint32_t pause, uint8_t repetitions);
};

#endif /* VIBRATION_H_ */
