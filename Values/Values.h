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
			if (EEPROM.read(VALUES_SET_ADDR) != 1) {
				// Values initiated flag
				EEPROM.write(VALUES_SET_ADDR, 1);
				// Set thresholds
				EEPROM.write(VOC_THRESH_ADDR, 1);
				EEPROM.write(UVI_THRESH_ADDR, 8);
				EEPROM.write(UVI_DUR_THRESH_ADDR, 10);
				EEPROM.write(TEMP_THRESH_ADDR, 30);
				EEPROM.write(CO2_THRESH_ADDR_HI, 0x05); // 0x0578 = 1400
				EEPROM.write(CO2_THRESH_ADDR_LO, 0x78);
				EEPROM.write(STEP_GOAL_ADDR_HI, 0x27); // 0x2710 = 10000
				EEPROM.write(STEP_GOAL_ADDR_LO, 0x10);
				// Set Flash storage indices
				EEPROM.write(CO2_FLASH__IDX_ADDR, 0);
				EEPROM.write(VOC_FLASH__IDX_ADDR, 0);
				EEPROM.write(UVI_FLASH__IDX_ADDR, 0);
				EEPROM.write(TEMP_FLASH__IDX_ADDR, 0);

				EEPROM.commit();
			}
		};
		~Values(void) {};

		// Default thresholds
		uint16_t co2Thresh;
		uint16_t vocThresh;
		uint8_t tempThresh;
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
		uint8_t co2_idx;
		uint16_t voc[VOC_STORAGE_SIZE] = {0}; // in ppb, bzw. mg/m3
		uint8_t voc_idx;
		uint8_t uvi[UVI_STORAGE_SIZE] = {0};   // UV index
		uint8_t uvi_idx;
		float temp[TEMP_STORAGE_SIZE] = {0}; // in degrees
		uint8_t temp_idx;
		uint16_t steps;

		// Set Threshold values
		void setCO2Thresh(uint16_t val);
		void setVOCThresh(uint16_t val);
		void setTempThresh(uint8_t val);
		void setStepGoal(uint16_t val);
		void setUVIThresh(uint8_t val);
		void setUVIDurationThresh(uint8_t val);

		// Get Thresholds...
		uint16_t getCO2Thresh(void);
		uint16_t getVOCThresh(void);
		uint8_t getTempThresh(void);
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
