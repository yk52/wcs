#ifndef DATATEST_H_
#define DATATEST_H_



#include "C:\Users\Joanna\Documents\MasterESE\3-Semester\WCS\wcs\Config\config.h"
#include <EEPROM.h>
#include <stdio.h>
using namespace std;
#include <string>
#include <iostream>

class DataTest {
	public:
		//constructors
		DataTest(void) {};
		~DataTest(void) {};

		uint16_t co2[CO2_STORAGE_SIZE] = {400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400};  	// in ppm. in flash memory, its divided by 100 to fit in a byte

		uint16_t voc[VOC_STORAGE_SIZE] = {500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500}; 	// in ppb, bzw. mg/m3

		uint8_t uvi[UVI_STORAGE_SIZE] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};   	// UV index

		float temp[TEMP_STORAGE_SIZE] = {20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20}; 					// in degrees

		uint16_t steps = 1000;

};


#endif /* DataTest_H_ */
