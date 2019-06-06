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


class ArduinoPedometer
{
	public:
		ArduinoPedometer() {};
		~ArduinoPedometer() {};

		float xval[15] = {0};
		float yval[15] = {0};
		float zval[15] = {0};

		float xavg;
		float yavg;
		float zavg;

		// Raw Ranges:
		// initialize to mid-range and allow calibration to
		// find the minimum and maximum for each axis
		int xRawMin = 512;
		int xRawMax = 512;

		int yRawMin = 512;
		int yRawMax = 512;

		int zRawMin = 512;
		int zRawMax = 512;

		int steps,flag=0;
		float threshhold=80.0;
		int getPedo();
		int readAxis(int, int);
		void setAverage();
		void calibrate();
		
	private:

};

#endif /* ADAFRUIT_ADXL335_H_ */
