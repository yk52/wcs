#ifndef VALUES_H_
#define VALUES_H_

#include "Arduino.h"

// #include "WProgram.h"

#include "C:\Users\Yumi\Desktop\wcs\config.h"
#include <EEPROM.h>
#include <stdio.h>
using namespace std;


class Values {
	public:
		//constructors
		Values(void) {
			EEPROM.begin(FLASH_SIZE);
		};
		~Values(void) {};

		// Default thresholds
		uint16_t co2Thresh;
		uint16_t vocThresh;
		float tempThresh;
		uint8_t uviThresh;
		uint8_t uviDurationThresh;
		uint8_t uviDuration;
		uint16_t stepGoal;

		// Thresholds exceeded warning storage array
		uint32_t warnings[WARNING_STORAGE_SIZE] = {0};
		uint8_t warn_idx;
		bool warningOverflow;

		// Storage arrays
		uint16_t co2[CO2_STORAGE_SIZE] = {0};  // in ppm
		uint16_t co2_idx;
		uint16_t voc[VOC_STORAGE_SIZE] = {0}; // in ppb, bzw. mg/m3
		uint16_t voc_idx;
		uint8_t uvi[UVI_STORAGE_SIZE] = {0};   // UV index
		uint16_t uvi_idx;
		float temp[TEMP_STORAGE_SIZE] = {0}; // in degrees
		uint16_t temp_idx;
		uint16_t steps;

		// Set Threshold values
		void setCO2Thresh(uint16_t val);
		void setVOCThresh(uint16_t val);
		void setTempThresh(float val);
		void setStepGoal(uint16_t val);
		void setUVIThresh(uint8_t val);
		void setUVIDurationThresh(uint8_t val);

		// Get Thresholds...
		uint16_t getCO2Thresh(void);
		uint16_t getVOCThresh(void);
		float getTempThresh(void);
		uint16_t getStepGoal(void);
		uint8_t getUVIThresh(void);
		uint8_t getUVIDurationThresh(void);

		// ...and most recent values
		uint16_t getLastCO2(void);
		uint16_t getLastVOC(void);
		float getLastTemp(void);
		uint16_t getLastStep(void);
		uint8_t getLastUVI(void);
		uint8_t getLastUVIDuration(void);

		// Store values and check at the same time.
		void storeCO2(uint16_t);
		void storeVOC(uint16_t);
		void storeTemp(float);
		bool storeSteps(uint16_t);
		void storeUVI(uint8_t);

		// String must look like: "UVthreshold:10"
		void processMessage(string);



	private:

};

#endif /* VALUES_H_ */
