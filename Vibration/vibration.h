#ifndef VIBRATION_H
#define VIBRATION_H

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h>
#include "C:\Users\Yumi\Desktop\wcs\config.h"
#include "C:\Users\Yumi\Desktop\wcs\Scheduler\scheduler.h"



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
		void on(void);
		void off(void);
		void check(void);
		void rhythm(uint32_t duration, uint32_t pause, uint8_t repetitions);
};

#endif /* VIBRATION_H_ */
