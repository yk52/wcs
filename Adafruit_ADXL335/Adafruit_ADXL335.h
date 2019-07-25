/*------------------------------------------------------------
This is the .h file for ARDUINO PEDOMETER Library
This is the property of Frugal Labs Tech Solutions Pvt. Ltd.
Please refer lisence.txt for complete details.
-------------------------------------------------------------*/

#ifndef ADAFRUIT_ADXL335_H_
#define ADAFRUIT_ADXL335_H_
#if ARDUINO >= 15
#include "Arduino.h"

#else
#include "WProgram.h"

#endif


class Adafruit_ADXL335
{
	public:
		Adafruit_ADXL335() {};
		~Adafruit_ADXL335() {};

		float zavg;

		int steps,flag = 0;
		float threshold=5000.0;
		float oldVector = 0;
		float diff[15]={0};
		int idx = 0;
		int getPedo();
		int readAxis(int, int);
		void calibrate();
		
	private:

};

#endif /* ADAFRUIT_ADXL335_H_ */
